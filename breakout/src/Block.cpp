#include "Block.h"
#include <iostream>
#include "sl.h"
#include <math.h>

namespace BlockSpace
{
	const int rows = 6;
	const int cols = 10;

	const int blocksToBreak = 10;
	int blocksBroken = 0;

	static Block blocks[rows][cols];

	void PowerBall(Ball& ball);
	Colors GetRandomColor();
	Block GetBlock(Vector2 offSet);
	void BlockBallCollision(Ball& ball, Block& square);

	void PowerBall(Ball& ball)
	{
		PowerUps power = (PowerUps)GetRandomNum((int)PowerUps::NoBounce, 0);

		BallSpace::SetPower(power, ball);
	}

	Colors GetRandomColor()
	{
		Colors color = RED;
		color = (Colors)GetRandomNum(PINK, 0);
		return color;
	}

	Block GetBlock(Vector2 offSet)
	{
		Block myBlock;
		myBlock.color = GetRandomColor();
		myBlock.height = 40;
		myBlock.width = screenWidth / cols;
		myBlock.pos.x = 0;
		myBlock.pos.y = 0;
		myBlock.offSet = offSet;
		myBlock.dead = false;
		return myBlock;
	}

	void BlockBallCollision(Ball& ball, Block& square)
	{
#pragma region CORNER_CALCULATIONS
		//SQUARE CORNERS		//	c1----------------c3
		Vector2 sCorner1; 		//	|				   |  
		Vector2 sCorner2;		//	|		center	   |  
		Vector2 sCorner3;		//	|				   |  
		Vector2 sCorner4;		//	c2----------------c4  

		//pos x and pos y of square is the center, thanks to sigil. I made these calculations to adaptate it
		sCorner1.x = square.pos.x - square.width / 2;
		sCorner1.y = square.pos.y + square.height / 2;

		sCorner2.x = sCorner1.x;
		sCorner2.y = square.pos.y - square.height / 2;

		sCorner3.x = square.pos.x + square.width / 2;
		sCorner3.y = sCorner1.y;

		sCorner4.x = sCorner3.x;
		sCorner4.y = sCorner2.y;
#pragma endregion

#pragma region DISTANCE_CALCULATIONS
		Distances calculations;

		calculations.pinPointX = (int)ball.pos.x;
		calculations.pinPointY = (int)ball.pos.y;


		if ((int)ball.pos.x <= (int)sCorner1.x)
			calculations.pinPointX = (int)sCorner1.x; //left

		else if ((int)ball.pos.x >= (int)sCorner3.x)
			calculations.pinPointX = (int)sCorner3.x; //right

		if ((int)ball.pos.y >= (int)sCorner1.y)
			calculations.pinPointY = (int)sCorner1.y; //top

		else if ((int)ball.pos.y <= (int)sCorner2.y)
			calculations.pinPointY = (int)sCorner2.y; //bottom

		calculations.distX = (int)ball.pos.x - calculations.pinPointX;
		calculations.distY = (int)ball.pos.y - calculations.pinPointY;
		calculations.distance = sqrt((calculations.distX * calculations.distX) + (calculations.distY * calculations.distY));

#pragma endregion


		if (calculations.distance < ball.radius)
		{
			square.dead = true;

			blocksBroken++;

			if (blocksBroken == blocksToBreak)
			{
				BallSpace::SetPower(PowerUps::None, ball);
				blocksBroken = 0;
			}
			else if(ball.currentPower == PowerUps::None)
				PowerBall(ball);
			
			if (ball.currentPower != PowerUps::NoBounce)
			{
				//Where does it come from? Is it partially inside the rect?
				//VERTICAL
				if (calculations.pinPointY == (int)sCorner1.y ||
					calculations.pinPointY == (int)sCorner2.y)
				{
					//top
					if (calculations.pinPointY == (int)sCorner1.y)
					{
						ball.pos.y = sCorner1.y + ball.radius * 2;

						if (ball.speed.y < 0)
							ball.speed.y *= -1;
					}

					//bottom
					if (calculations.pinPointY == (int)sCorner2.y)
					{
						ball.pos.y = sCorner2.y - ball.radius * 2;

						if (ball.speed.y > 0)
							ball.speed.y *= -1;
					}
				}
				//HORIZONTAL
				else if (calculations.pinPointX == (int)sCorner1.x ||
					calculations.pinPointX == (int)sCorner3.x)
				{
					//left
					if (calculations.pinPointX == (int)sCorner1.x)
						ball.pos.x = sCorner1.x - ball.radius * 2;
					//right
					else if (calculations.pinPointX == (int)sCorner3.x)
						ball.pos.x = sCorner3.x + ball.radius * 2;

					ball.speed.x *= -1;
				}
			}
		}
	}

	bool AreBlocksGone()
	{
		for (int y = rows - 1; y >= 0; y--)
			for (int x = 0; x < cols; x++)
				if (!blocks[y][x].dead)
					return false;

		return true;
	}

	void CreateBlocks()
	{
		int offSetX = 0;
		int offSetY = screenHeight * 4 / 5;
		Vector2 thisOffSet = { offSetX,offSetY };

		for (int y = 0; y < rows; y++)
		{
			for (int x = 0; x < cols; x++)
			{
				blocks[y][x] = GetBlock(thisOffSet);
				blocks[y][x].pos.x = blocks[y][x].offSet.x + blocks[y][x].width / 2;
				blocks[y][x].pos.y = blocks[y][x].offSet.y - blocks[y][x].height / 2;

				thisOffSet.x += blocks[y][x].width;
			}

			thisOffSet.y -= blocks[y][0].height;
			thisOffSet.x = 0;
		}
	}

	void UpdateBlocks(Ball& ball)
	{
		for (int y = rows - 1; y >= 0; y--)
			for (int x = 0; x < cols; x++)
				if (!blocks[y][x].dead)
					BlockBallCollision(ball, blocks[y][x]);

	}

	void DrawBlocks()
	{
		for (int y = 0; y < rows; y++)
			for (int x = 0; x < cols; x++)
				if (!blocks[y][x].dead)
					DrawBlock(blocks[y][x]);
	}

	void DrawBlock(Block block)
	{
		SetForeColor(WHITE);
		//SetForeColor(block.color);
		slRectangleFill(block.pos.x, block.pos.y, block.width, block.height);
		SetForeColor(PURPLE);
		slRectangleOutline(block.pos.x, block.pos.y, block.width, block.height);
	}

}

