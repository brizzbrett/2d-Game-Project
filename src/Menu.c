#include "Menu.h"
#include "Camera.h"

int Menu_StartGame()
{
	const Uint8 *keys;

	keys = SDL_GetKeyboardState(NULL);

	if(keys[SDL_SCANCODE_SPACE])
		return 1;

	return 0;
}
Menu *Menu_Init()
{
	Menu *menu;
	menu = (Menu *)malloc(sizeof(Menu));
	memset(menu,0,sizeof(Menu));

	menu->bg = sprite_Load("images/menu.png",1000,703);

	return menu;

}

void Menu_Draw(Menu *menu)
{
	Vec2d pos = Camera_GetPosition();
	sprite_Draw(menu->bg,0,Graphics_GetActiveRenderer(),pos);
}

