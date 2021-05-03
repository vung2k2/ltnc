#include "Game.h"



Game::Game()
{
	_windowTitle = "Tetris";
	_windowWidth = 800;
	_windowHeight = 800;
	_boardPositionX = 50;
	_boardPositionY = 50;
}

Game::Game(std::string windowTitle, int windowWidth, int windowHeight)
{
	_windowTitle = windowTitle;
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
}


Game::~Game()
{
}

//Khởi tạo cửa sổ
bool Game::Game_Init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Could not initialize SDL: "<< SDL_GetError();
		success = false;
	}
	else
	{
		_window = SDL_CreateWindow(_windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowWidth, _windowHeight, SDL_WINDOW_SHOWN);
		if (_window == NULL)
		{
			std::cout << "Could not create window.: "<< SDL_GetError();
			success = false;
		}
		else
		{
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (_renderer == NULL)
			{
				std::cout<<"Could not initialize renderer: "<< SDL_GetError();
				success = 0;
			}
			else
			{
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "Could not initialize SDL_Image: " << IMG_GetError();
					success = false;
				}
				else
				{
					_windowSurface = SDL_GetWindowSurface(_window);

					SDL_FillRect(_windowSurface, NULL, SDL_MapRGB(_windowSurface->format, 0xFF, 0xFF, 0xFF));
				}
			}
		}
	}
	_running = true;

	Game_LoadMedia();

	return success;
}

void Game::Game_LoadMedia()
{
	_blockTexture.loadFromFile("image/block.png", _renderer, 0.35f);
	_board.setTexture(_blockTexture);
}

void Game::Game_Run()
{
	while (_running)
	{
		while (SDL_PollEvent(&_e) != 0)
		{
			if (_e.type == SDL_QUIT) _running = false;
			_board.handleInput(_e);
		}

		_board.update();
		Game_Draw();
		if(_board.GameOver()){
            Game_LoadGameOver();
            SDL_Delay(4000);
            _board.resetGame();
        }
	}
}

void Game::Game_LoadGameOver(){
    SDL_Surface * image = SDL_LoadBMP("image/gameOver.bmp");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(_renderer, image);
    SDL_RenderCopy(_renderer, texture, NULL, NULL);
    SDL_RenderPresent(_renderer);
}

//Vẽ ra màn hình
void Game::Game_Draw()
{
	SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(_renderer);
    //Game_LoadMap();

    _board.draw(_renderer);

	SDL_RenderPresent(_renderer);
}

