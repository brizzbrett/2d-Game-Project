#include "Level.h"
#include <cmath>
#include <time.h>
#include <random>

Node *head;

/**
 * @brief	Level load.
 */
void Level_Load()
{
	head = Node_New();
	head->width = 51200;
	head->height = 28800;
	srand(time(NULL));

	Node_RecursiveSubDivide(head, 5);
	Room_RecursiveCreateRoom(head);

}