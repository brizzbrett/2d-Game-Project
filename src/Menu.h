#ifndef __MENU_H__
#define __MENU_H__

#include "Entity.h"

typedef struct Menu_S
{
	Sprite *bg;

}Menu;

Menu *Menu_Init();
void Menu_Draw(Menu *menu);

int Menu_StartGame();



#endif