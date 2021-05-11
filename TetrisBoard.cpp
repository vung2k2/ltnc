#include "TetrisBoard.h"



TetrisBoard::TetrisBoard()
{//Tọa độ khung
	_posX = 50;
	_posY = 50;
	resetGame();
//Tọa độ khối
	_piecePosY = -2;
	_piecePosX = 4;
}


TetrisBoard::~TetrisBoard()
{
}

// Tạo bảng
void TetrisBoard::initBoard()
{
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		for (int j = 0; j < BOARD_HEIGHT; j++)
		{
			if (i == 0) _boardArray[i][j] = BORDER;
			else if (i == BOARD_WIDTH - 1) _boardArray[i][j] = BORDER;
			else if (j == BOARD_HEIGHT - 1) _boardArray[i][j] = BORDER;
			else _boardArray[i][j] = EMPTY;
		}
	}
}

void TetrisBoard::setTexture(Texture texture)
{
	_block.setTexture(texture.getTexture());
	_block.setWidth(texture.getWidth());
	_block.setHeight(texture.getHeight());
}

void TetrisBoard::draw(SDL_Renderer* renderer)
{

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect rect{ _posX, _posY, BOARD_WIDTH*BLOCK_SIZE + 2, BOARD_HEIGHT*BLOCK_SIZE + 2 };
	SDL_RenderDrawRect(renderer, &rect);

	//Vẽ khối
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (piece.getPieceData(i, j) != EMPTY)
			{
				setColor(piece.getPieceData(i, j));
				_block.render(_posX + (_piecePosX + j) * BLOCK_SIZE + 1, _posY + (_piecePosY + i) * BLOCK_SIZE + 1, renderer, NULL);

			}
		}
	}

	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (piece0.getPieceData(i, j) != EMPTY)
			{
				setColor(piece0.getPieceData(i, j));

				_block.render( j * BLOCK_SIZE + 570, i * BLOCK_SIZE + 200, renderer, NULL);

			}
		}
	}

	//Vẽ khung
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		for (int j = 0; j < BOARD_HEIGHT; j++)
		{
			if (_boardArray[i][j] != EMPTY)
			{
				setColor(_boardArray[i][j]);
				_block.render(_posX + i * BLOCK_SIZE + 1, _posY + j * BLOCK_SIZE + 1, renderer, NULL);
			}
		}
	}
	//In điểm
	loadText("SCORE: ",renderer,30,550,500);
	loadText(int_to_string(score),renderer,30,680,500);
	//In độ khó
	if(PER_FRAME == 10) loadText("HARD",renderer,30,580,400);
	if(PER_FRAME == 25) loadText("MEDIUM",renderer,30,580,400);
	if(PER_FRAME == 40) loadText("EASY",renderer,30,580,400);

}

bool TetrisBoard::isOccupied(int i, int j)
{
	if (_boardArray[i][j]) return true;
	else return false;
}


//Làm mới bảng
void TetrisBoard::update()
{
	if ((_frame % PER_FRAME) == 0 )
	{
		if(canPlace(_piecePosX, _piecePosY + 1))
			_piecePosY += 1;
		else
		{
			for (int i = 0; i < PIECE_SIZE; i++)
			{
				for (int j = 0; j < PIECE_SIZE; j++)
				{
					if (piece.getPieceData(i, j) != EMPTY)
						_boardArray[_piecePosX + j][_piecePosY + i] = piece.getPieceData(i, j);
				}

				if (isLineFull(i + _piecePosY))
				{
					for (int j = 1; j < BOARD_WIDTH - 1; j++)
						_boardArray[j][_piecePosY + i] = WHITE;

					shiftBoard(i + _piecePosY);
					score = score + 10;;

				}
			}

			_piecePosX = 4;
			_piecePosY = 0;
			piece=piece0;
			piece0.randomizePiece();
		}
	}
	_frame++;
	if (_frame >= PER_FRAME) _frame = 0;
}


//Xử lý đầu vào
void TetrisBoard::handleInput(SDL_Event e)
{
	ArrowKeys input = _input.getPress(e);

	switch (input)
	{
	case RIGHT:
		if (canPlace(_piecePosX + 1, _piecePosY)) _piecePosX++;
		break;
	case LEFT:
		if (canPlace(_piecePosX - 1, _piecePosY)) _piecePosX--;
		break;
	case DOWN:
		if (canPlace(_piecePosX, _piecePosY + 1)) _piecePosY++;
		break;
	case UP:
		piece.rotatePiece();
		if (!canRotate())
			piece.reverseRotation();
		break;
	case RESET:
		resetGame();
		break;
    case EXIT:
        SDL_Quit();
        break;
    case EASY:
        PER_FRAME = 40;
        break;
    case MEDIUM:
        PER_FRAME = 25;
        break;
    case HARD:
        PER_FRAME = 10;
        break;
	}
}


//Kiểm tra xem có phải dòng đầy đủ ko
bool TetrisBoard::isLineFull(int i)
{
	if (i == BOARD_HEIGHT - 1) return false;

	for (int j = 0; j < BOARD_WIDTH; j++)
	{
		if (_boardArray[j][i] == EMPTY) return false;
	}

	return true;
}


//Kiểm tra xem khối có vượt qua giới hạn ko
bool TetrisBoard::canPlace(int newPosX, int newPosY)
{
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (newPosX + j >= 0 && newPosX + j < BOARD_WIDTH)
			{
				if (newPosY + i >= 0 && newPosY + i < BOARD_HEIGHT)
				{
					if (piece.getPieceData(i, j) != EMPTY && _boardArray[newPosX + j][newPosY + i] != EMPTY) return false;
				}
			}
		}
	}

	return true;
}


//Xóa đi hàng đã đầy trong bảng
void TetrisBoard::shiftBoard(int row)
{
	for(int i = row; i > 0; i--)
		for (int j = 1; j < BOARD_WIDTH - 1; j++)
		{
			_boardArray[j][i] = _boardArray[j][i - 1];
		}
}

bool TetrisBoard::canRotate()
{
	return canPlace(_piecePosX, _piecePosY);
}

void TetrisBoard::resetGame()
{
    score=0;
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		for (int j = 0; j < BOARD_HEIGHT; j++)
		{
			if (i == 0) _boardArray[i][j] = BORDER;
			else if (i == BOARD_WIDTH - 1) _boardArray[i][j] = BORDER;
			else if (j == BOARD_HEIGHT - 1) _boardArray[i][j] = BORDER;
			else _boardArray[i][j] = EMPTY;
		}
	}

	_piecePosX = 4;
	_piecePosY = -1;
	_frame = 0;

	piece.randomizePiece();
}

bool TetrisBoard::GameOver()
{
   for (int j = 3; j < 8; j++)
	{
		if (_boardArray[j][1] != EMPTY) return true;
	}
	return false;
}


//Bảng màu
void TetrisBoard::setColor(Color color)
{
	switch (color)
	{
		case RED:
			//_block.setColor(160, 0, 0);
			_block.setColor(255, 0, 0);
			break;
		case GREEN:
			//_block.setColor(180, 0, 0);
			_block.setColor(0, 255, 0);
			break;
		case BLUE:
			//_block.setColor(140, 0, 0);
			_block.setColor(0, 0, 255);
			break;
		case YELLOW:
			//_block.setColor(120, 0, 0);
			_block.setColor(255, 255, 0);
			break;
		case MAGENTA:
			//_block.setColor(100, 0, 0);
			_block.setColor(255, 0, 255);
			break;
		case ORANGE:
			//_block.setColor(80, 0, 0);
			_block.setColor(255, 140, 0);
			break;
		case PINK:
			//_block.setColor(60, 0, 0);
			_block.setColor(255, 20, 147);
			break;
		case BORDER:
			_block.setColor(160, 160, 160);
			break;
		case WHITE:
			_block.setColor(255, 255, 255);
			break;
		default:
			_block.setColor(0xFF, 0xFF, 0xFF);
			break;
	}
}

std::string int_to_string(int x){
    std::stringstream ss;
    ss<<x;
    std::string s;
    ss>>s;
  return s;
}

void loadText(std::string s, SDL_Renderer* renderer, int _size, int x, int y){
    TTF_Font* font = TTF_OpenFont("VAVOBI.ttf", _size);
    SDL_Color fg = { 255, 0, 0 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, s.c_str(), fg);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect srcRest;
	SDL_Rect desRect;
	TTF_SizeText(font, s.c_str(), &srcRest.w, &srcRest.h);

	srcRest.x = 0;
	srcRest.y = 0;

	desRect.x = x;
	desRect.y = y;

	desRect.w = srcRest.w;
	desRect.h = srcRest.h;
	SDL_RenderCopy(renderer, texture, &srcRest, &desRect);
}

void TetrisBoard::luu_diem(){
    std::ifstream infile;
    infile.open("image/score.txt");
    infile >> scoreMax;
    infile.close();
    if(score>scoreMax){
        scoreMax=score;
        std::ofstream outfile;
        outfile.open("image/score.txt");
        outfile << scoreMax ;
        outfile.close();
    }

}
