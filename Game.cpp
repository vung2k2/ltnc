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
	if (TTF_Init() < 0)
	{
		SDL_Log("%s", TTF_GetError());
		return -1;
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
//Menu
    running = true;
    while(running){
        load_image("image/Menu.bmp");
        SDL_RenderPresent(_renderer);

        if(SDL_PollEvent(&_e) != 0){
           if(_e.key.keysym.sym==SDLK_SPACE
              || _e.key.keysym.sym==SDLK_ESCAPE
              || _e.key.keysym.sym==SDLK_1
              || _e.key.keysym.sym==SDLK_2
              || _e.key.keysym.sym==SDLK_3 )
            {
              _board.handleInput(_e);
              running=false;
            }
        }
    }
        running = true;



//Game
	while (_running)
	{
		while (SDL_PollEvent(&_e) != 0)
		{
			if (_e.type == SDL_QUIT) _running = false;
			_board.handleInput(_e);
		}
		_board.update();
		Game_Draw();

//Game over
		if(_board.GameOver()){
            _board.luu_diem();
            while(running){
                load_image("image/gameOver.bmp");
                loadText(int_to_string(_board.scoreMax),_renderer,30,400,545);
                loadText(int_to_string(_board.score),_renderer,30,400,485);
                SDL_RenderPresent(_renderer);

                if(SDL_PollEvent(&_e) != 0){
                    if(_e.key.keysym.sym == SDLK_ESCAPE || _e.key.keysym.sym == SDLK_r){
                        _board.handleInput(_e);
                        running=false;
                    }
                }
            }
            running = true;
        }
}
}
void Game::load_image(std::string file){
    SDL_Surface * image = SDL_LoadBMP(file.c_str());
    SDL_Texture * texture = SDL_CreateTextureFromSurface(_renderer, image);
    SDL_RenderCopy(_renderer, texture, NULL, NULL);
}

//Vẽ ra màn hình
void Game::Game_Draw()
{
	SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(_renderer);
    _board.draw(_renderer);
	SDL_RenderPresent(_renderer);
}

