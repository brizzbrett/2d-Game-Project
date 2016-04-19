#include "Level.h"

GList *NList;
GList *leafList;
int id = 64;

Node *Node_New(int x, int y, int width, int height, Node *parent)
{
	Node *n = (Node *)malloc(sizeof(Node));
	memset(n,0,sizeof(Node));

	n->id = ++id;
	n->pos.x = x;
	n->pos.y = y;
	n->width = width;
	n->height = height;
	n->parent = parent;
	NList = g_list_append(NList, n);
	return n;
}

void Node_SubDivide(Node *n)
{
	int size = 0;
	Node *a;
	Node *b;

	if(n == NULL)
	{
		return;
	}
	n->split = rand() % 2;

	if(n->split == SPLIT_VERTICAL)
	{
		size = n->width/2;

		if(size < 1600)
		{
			return;
		}
		a = Node_New(n->pos.x,n->pos.y,size,n->height,n);
		b = Node_New(n->pos.x+size,n->pos.y,n->width-size, n->height,n);

		n->left = a;
		n->right = b;
	}
	else
	{
		size = n->height/2;

		if(size < 900)
		{
			return;
		}
		a = Node_New(n->pos.x,n->pos.y,n->width,size,n);
		b = Node_New(n->pos.x,n->pos.y+size,n->width,n->height-size,n);

		n->top = a;
		n->bottom = b;
	}
	
}	
GList *NodeList_Get()
{
	return NList;
}
void Node_RecursiveSubDivide(Node *n, int count)
{	

	if(count <= 0)
	{
		return;
	}

	Node_SubDivide(n);
	if(n->left)
	{
		Node_RecursiveSubDivide(n->left, count-1);
	}
	if(n->right)
	{
		Node_RecursiveSubDivide(n->right, count-1);
	}

}

GList *LeafList_Get()
{
	return leafList;
}
/*Room *Room_Get(Node *n, int gsplit ,int psplit)
{	
	Room *lRoom = NULL;
	Room *rRoom = NULL;

	if(n->room)
	{
		return n->room;
	}
	else
	{		
		if (n->right)
		{
			rRoom = Room_Get(n->right, gsplit, psplit);
		}
		if (n->left)
		{
			lRoom = Room_Get(n->left, gsplit, psplit);
		}

		if(!lRoom && !rRoom)
		{
			return NULL;
		}		
		else if(!lRoom || gsplit == psplit)
		{
			return rRoom;
		}
		else
		{
			return lRoom;
		}
	}
}

void Room_RecursiveCreateRoom(Node *n, char *file)
{
	if(n->left || n->right)
	{		
		if(n->left)
		{	
			Room_RecursiveCreateRoom(n->left, file);
		}
		if(n->right)
		{
			Room_RecursiveCreateRoom(n->right, file);
		}
		if(n->left && n->right) // if node has both left and right children, 
								// traverse these nodes until you get a room from both nodes and link them
		{
			Room_Link(Room_Get(n->left, n->split, n->left->split), Room_Get(n->right,n->split, n->right->split), n->split);
		}
	}
	else
	{
		leafList = g_list_append(leafList, n);
		n->room = Room_New(n->pos, file, 2, n);		
	}
}*/
/////////////////////////////////////////////////////////
//					MEMORY MANAGEMENT				   //
/////////////////////////////////////////////////////////

void Node_Free(Node **n)
{
	//Entity *self; /**<alias for *ent*/
	if(!(*n))return;
	if(!n)return;
	if((*n)->room)
	{
		if((*n)->room->image)
		{
			sprite_Free(&(*n)->room->image);
			(*n)->room->image = NULL;
		}
		(*n)->room = NULL;
	}
	*n = NULL;
	n = NULL;

}

void Node_CloseSystem()
{
	GList *g;
	Node *n; /**<alias for *ent*/
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	for(g = NList; g != NULL; g = g->next)
	{
		n = (Node *)(g->data);
        Node_Free(&n);
	}

	free(g);
	NList = NULL;
}

void Level_Load(Uint8 levelType)
{
	GList *g;
	Room *r;
	Node *l;
	char *data;
	FILE *f;
	FILE *log;
	long len;
	char n, s, w, e;
	Music *nightmare_music;
	cJSON *json, *root, *obj;

	if(!level)return;

	f = fopen(level, "rb");
	log = fopen("def/log.txt","w");
	if(!f)return;

	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);

	data=(char*)malloc(len+1);
	fread(data,1,len,f);
	fclose(f);

	json = cJSON_Parse(data);
	if(!json)return;
	if(levelType == HUB)
	{
		Hub_Create(level);
	}
	else if(levelType == NIGHTMARE)
	{
		nightmare_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(nightmare_music);
		root = cJSON_GetObjectItem(json, "nightmare");
		if(!root)return;
		obj = cJSON_GetObjectItem(root, "level load");
		head = Node_New(0,0,cJSON_GetObjectItem(obj, "h width")->valueint,cJSON_GetObjectItem(obj, "h height")->valueint, NULL);

		srand(time(NULL));

		Node_RecursiveSubDivide(head, 4);
		Room_RecursiveCreateRoom(head, cJSON_GetObjectItem(obj, "room look")->valuestring);
		
		fprintf(log,"******NODES******");
		for (g = NodeList_Get(); g != NULL; g = g->next)
		{
			l = (Node *)(g->data);
			if(l->left || l->right)
				if(l->parent)
					fprintf(log,
						"\nNODE %c----PARENT %c-- SPLIT: %i\n\tPOSITION-- X: %.00f, Y: %.00f \n\tSIZE-- W: %i, H:%i\n\tChildren: %c, %c", 
						l->id, l->parent->id, l->parent->split, l->pos.x, l->pos.y, l->width, l->height, l->left->id, l->right->id);
				else
					fprintf(log,
						"\nNODE %c----PARENT N/A-- SPLIT: N/A\n\tPOSITION-- X: %.00f, Y: %.00f \n\tSIZE-- W: %i, H:%i\n\tChildren: %c, %c", 
						l->id, l->pos.x, l->pos.y, l->width, l->height, l->left->id, l->right->id);
			else
				fprintf(log,
					"\nNODE %c----PARENT %c-- SPLIT: %i\n\tPOSITION-- X: %.00f, Y: %.00f \n\tSIZE-- W: %i, H:%i", 
					l->id, l->parent->id, l->parent->split, l->pos.x, l->pos.y, l->width, l->height);
		}
		fprintf(log,"\n******LEAFS******");
		for (g = LeafList_Get(); g != NULL; g = g->next)
		{
			l = (Node *)(g->data);
			if(l->room->north != NULL)
				n = 'N';
			else
				n = ' ';
			if(l->room->south != NULL)
				s = 'S';
			else
				s = ' ';
			if(l->room->east != NULL)
				e = 'E';
			else
				e = ' ';
			if(l->room->west != NULL)
				w = 'W';
			else
				w = ' ';
			fprintf(log,
				"\nNODE %c----PARENT %c-- SPLIT: %i----GP %c-- SPLIT: %i\n\tPOSITION-- X: %.00f, Y: %.00f\n\tRoom: %c--\n\t\tPOSITION-- X: %.00f, Y: %.00f\n\t\tSIZE-- W: %.00f, H:%.00f\n\t\tDOORS-- %c%c%c%c", 
				l->id,l->parent->id,l->parent->split, l->parent->parent->id,l->parent->parent->split, l->pos.x,l->pos.y, l->room->val+65,l->room->pos.x,l->room->pos.y, l->room->size.x, l->room->size.y, n, s, e, w);
		}
		fclose(log);
		//Door_Placer();
		atexit(Node_CloseSystem);
	}
}