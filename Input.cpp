#include "Input.h"



Input::Input()
{
	_upPressed = false;
	_downPressed = false;
	_leftPressed = false;
	_rightPressed = false;

	_leftFrame = 0;
	_rightFrame = 0;

	for (int i = 0; i < 322; i++) { // init them all to false
		KEYS[i] = false;
	}
}


Input::~Input()
{
}

ArrowKeys Input::getPress(SDL_Event e)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
			case SDLK_r:
			    return RESET;

			case SDLK_UP:
				return UP;

			case SDLK_DOWN:
				return DOWN;

			case SDLK_LEFT:
				return LEFT;

			case SDLK_RIGHT:
				return RIGHT;

            case SDLK_ESCAPE:
                return EXIT;
		}
	}
}
