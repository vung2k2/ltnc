#pragma once
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <iostream>

#include "Texture.h"
#include "Color.h"
#include "Input.h"
#include "Piece.h"


#define BOARD_WIDTH 12
#define BOARD_HEIGHT 21
#define BLOCK_SIZE 35
#define PER_FRAME 25

class TetrisBoard
{
	public:
		TetrisBoard();
		~TetrisBoard();

		void randomizeBoard();
		void initBoard();
		void draw(SDL_Renderer* renderer);
		bool isOccupied(int i, int j);
		void setTexture(Texture texture);
		void setColor(Color color);
		void update();
		void shiftBoard(int row);
		void resetGame();
		void handleInput(SDL_Event e);
        bool GameOver();

	private:
		bool canPlace(int newPosX, int newPosY);
		bool canRotate();
		bool isLineFull(int i);

		Input _input;

		int _frame;
		Texture _block;
		int _posX;
		int _posY;
		int _piecePosX;
		int _piecePosY;
		Piece piece,piece0;
		Color _piece[4][4];
		Color _boardArray[BOARD_WIDTH][BOARD_HEIGHT];
};

