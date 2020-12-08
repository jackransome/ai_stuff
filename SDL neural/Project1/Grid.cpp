#include "Grid.h"
#include <GameEngine/GameEngine.h>
#include <iostream>
#include <functional>
#include <algorithm>

Grid::Grid(GameEngine::SpriteBatch* _sb)
{
	sb = _sb;
	setStart(25, 25);
	setEnd(10, 10);
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			grid[i][j] = Cell();
			grid[i][j].x = i;
			grid[i][j].y = j;
			grid[i][j].isNew = true;
			grid[i][j].isStart = false;
			grid[i][j].isEnd = false;
			grid[i][j].isWall = false;
			grid[i][j].score = 100000;
			grid[i][j].direction = up;
		}
	}
	reset();
	setStart(25, 25);
	setEnd(10, 10);
}

Grid::~Grid()
{
}

void Grid::draw(int _x, int _y)
{
	_x -= (gridSize + gridSize * 10)/2;
	_y -=( gridSize + gridSize * 10)/2;
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			GameEngine::Color color;
			if (grid[i][j].isWall)
			{
				color.r = 200;
				color.g = 200;
				color.b = 200;
				color.a = 255;
			}
			else
			{
				color.r = 0;
				color.g = 0;
				color.b = 0;
				color.a = 0;
				if (grid[i][j].searched)
				{
					color.r = 60;
					color.g = 60;
					color.b = 60;
				}
				if (grid[i][j].finished)
				{
					color.r = 0;
					color.g = 0;
					color.b = 0;
				}
				if (grid[i][j].isPath)
				{
					color.r = 0;
					color.g = 255;
					color.b = 0;
				}

			}

			if (searched.size() > 0)
			{
				color.r += (255 - color.r)*(1 - float(grid[i][j].score) / searched[0]->score);
				color.b += (255 - color.b)*(float(grid[i][j].score) / searched[0]->score);
			}
			else
			{
				color.r += (255 - color.r)*(1 - float(grid[i][j].score) / 180);
				color.b += (255 - color.b)*(float(grid[i][j].score) / 180);
			}
			if (grid[i][j].isWall)
			{
				color.r = 200;
				color.g = 200;
				color.b = 200;
				color.a = 255;
			}
			if (grid[i][j].score > 10000 && !grid[i][j].isWall)
			{
				color.r = 255;
				color.g = 255;
				color.b = 255;
				color.a = 255;
			}
			if (grid[i][j].isEnd | grid[i][j].isStart)
			{
				color.r = 0;
				color.g = 0;
				color.b = 0;
				color.a = 0;
			}
			GameEngine::drawRect(_x + i*10 + i, _y + j*10 + j, cellSize, cellSize, 1, color, sb);
		}
	}
}
void Grid::reset()
{

	setStart(startx, starty);
	setEnd(endx, endy);
	searched.push_back(&grid[int(startx)][int(starty)]);
	isEndFound = false;

	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			grid[i][j].isPath = false;
			grid[i][j].finished = false;
			grid[i][j].searched = false;
			grid[i][j].isNew = true;
			grid[i][j].score = 100000;
		}
	}
	searched.clear();
	searched.push_back(&grid[int(startx)][int(starty)]);
	grid[int(startx)][int(starty)].isNew = false;
	grid[int(startx)][int(starty)].score = 0;
	isEndFound = false;
	x = endx;
	y = endy;
	isFinished = false;
	progress = 0;
	//system("cls");
	//std::cout << "STARTING CELL SCORE EVALUATION" << std::endl;
	//std::cout << "CELLS EVALUATED:";
}
void Grid::addWall(int _x, int _y)
{
	grid[_x][_y].isWall = true;
}
void Grid::removeWall(int _x, int _y)
{
	grid[_x][_y].isWall = false;
}
float Grid::heuristic(int _x1, int _y1)
{
	return abs(_x1 - endx) + abs(_y1 - endy);
	//return sqrt(pow(abs(_x1 - endx), 2) + pow(abs(_y1 - endy), 2));
	//return abs(_x1 - endx) + abs(_y1 - endy)+ sqrt(pow(abs(_x1 - endx), 2) + pow(abs(_y1 - endy), 2));
}
bool cmd(const Cell* c1, const Cell* c2)
{
	return c1->priority < c2->priority; 
	
}
void Grid::runAI()
{
	//set up
	int cellsEvaluated = 0;

	//setting directions
	int steps = 0;
	while (searched.size() > 0 && !isEndFound && steps < 1)
	{
		int i = 0;
		while (searched.size() > 0 && steps < 10)
		{
			steps++;
			//enable for proper A*
			std::sort(searched.begin(), searched.end(), cmd);
			if (!searched[i]->finished && !searched[i]->isNew) {
				if (searched[i]->isEnd)
				{
					isEndFound = true;
					break;
				}
				searched[i]->finished = true;
				///////////////
				///////////////
				///////////////
				///////////////
				if (searched[i]->x > 0 && !grid[int(searched[i]->x - 1)][int(searched[i]->y)].isWall)
				{
					if (grid[int(searched[i]->x - 1)][int(searched[i]->y)].score > searched[i]->score + 1) {
						if (!grid[int(searched[i]->x - 1)][int(searched[i]->y)].searched)
						{
							searched.push_back(&grid[int(searched[i]->x - 1)][int(searched[i]->y)]);
						}
						searched.push_back(&grid[int(searched[i]->x - 1)][int(searched[i]->y)]);
						grid[int(searched[i]->x - 1)][int(searched[i]->y)].searched = true;
						grid[int(searched[i]->x - 1)][int(searched[i]->y)].direction = right;
						grid[int(searched[i]->x - 1)][int(searched[i]->y)].score = searched[i]->score + 1;
						grid[int(searched[i]->x - 1)][int(searched[i]->y)].priority = heuristic(searched[i]->x - 1, searched[i]->y) + grid[int(searched[i]->x - 1)][int(searched[i]->y)].score/200;
						cellsEvaluated++;
					}
					else if (grid[int(searched[i]->x - 1)][int(searched[i]->y)].score < searched[i]->score)
					{
						searched[i]->direction = left;
					}
				}
				if (searched[i]->x < gridSize - 1 && !grid[int(searched[i]->x + 1)][int(searched[i]->y)].isWall)
				{
					if (grid[int(searched[i]->x + 1)][int(searched[i]->y)].score > searched[i]->score + 1){
						if (!grid[int(searched[i]->x + 1)][int(searched[i]->y)].searched)
						{
							searched.push_back(&grid[int(searched[i]->x + 1)][int(searched[i]->y)]);
						}
						grid[int(searched[i]->x + 1)][int(searched[i]->y)].searched = true;
						grid[int(searched[i]->x + 1)][int(searched[i]->y)].direction = left;
						grid[int(searched[i]->x + 1)][int(searched[i]->y)].score = searched[i]->score + 1;
						grid[int(searched[i]->x + 1)][int(searched[i]->y)].priority = heuristic(searched[i]->x + 1, searched[i]->y) + grid[int(searched[i]->x + 1)][int(searched[i]->y)].score/200;
						cellsEvaluated++;
					}
					else if (grid[int(searched[i]->x + 1)][int(searched[i]->y)].score < searched[i]->score)
					{
						searched[i]->direction = right;
					}
				}
				if (searched[i]->y > 0  &&  !grid[int(searched[i]->x)][int(searched[i]->y-1)].isWall)
				{
					if (grid[int(searched[i]->x)][int(searched[i]->y - 1)].score > searched[i]->score + 1) {
						if (!grid[int(searched[i]->x)][int(searched[i]->y + 1)].searched)
						{
							searched.push_back(&grid[int(searched[i]->x)][int(searched[i]->y + 1)]);
						}
						searched.push_back(&grid[int(searched[i]->x)][int(searched[i]->y - 1)]);
						grid[int(searched[i]->x)][int(searched[i]->y - 1)].searched = true;
						grid[int(searched[i]->x)][int(searched[i]->y - 1)].direction = up;
						grid[int(searched[i]->x)][int(searched[i]->y - 1)].score = searched[i]->score + 1;
						grid[int(searched[i]->x)][int(searched[i]->y - 1)].priority = heuristic(searched[i]->x, searched[i]->y - 1) + grid[int(searched[i]->x)][int(searched[i]->y - 1)].score/200;
						cellsEvaluated++;
					}
					else if (grid[int(searched[i]->x)][int(searched[i]->y - 1)].score < searched[i]->score)
					{
						searched[i]->direction = down;
					}
				}
				if (searched[i]->y < gridSize - 1  && !grid[int(searched[i]->x)][int(searched[i]->y+1)].isWall)
				{
					if (grid[int(searched[i]->x)][int(searched[i]->y + 1)].score > searched[i]->score + 1) {
						if (!grid[int(searched[i]->x)][int(searched[i]->y - 1)].searched)
						{
							searched.push_back(&grid[int(searched[i]->x)][int(searched[i]->y - 1)]);
						}
						searched.push_back(&grid[int(searched[i]->x)][int(searched[i]->y + 1)]);
						grid[int(searched[i]->x)][int(searched[i]->y + 1)].searched = true;
						grid[int(searched[i]->x)][int(searched[i]->y + 1)].direction = down;
						grid[int(searched[i]->x)][int(searched[i]->y + 1)].score = searched[i]->score + 1;
						grid[int(searched[i]->x)][int(searched[i]->y + 1)].priority = heuristic(searched[i]->x, searched[i]->y + 1) + grid[int(searched[i]->x)][int(searched[i]->y + 1)].score/200;
						cellsEvaluated++;
					}
					else if (grid[int(searched[i]->x)][int(searched[i]->y + 1)].score < searched[i]->score)
					{
						searched[i]->direction = up;
					}
				}
				if (isEndFound)
				{
					break;
				}
				//searched[i]->searched = false;
				searched.erase(searched.begin() + i);
				//i--;
			}
			if (!isEndFound) {
				//std::cout <<  int(100*progress/pow(gridSize,2)) << "%  ";
				progress += cellsEvaluated;
				for (int j = 0; j < searched.size(); j++)
				{
					searched[j]->isNew = false;
					if (searched[j]->finished == true)
					{
						searched.erase(searched.begin() + j);
						j--;
					}
				}
			}
		}

	}

	//getting path
	if (isEndFound)
	{
		while (!isFinished&& steps < 5)
		{
			steps++;
			if (x == startx && y == starty)
			{
				isFinished = true;
			}
			else{
				grid[x][y].isPath = true;
				if (grid[x][y].direction == up)
				{
					y++;
				}
				else if (grid[x][y].direction == right)
				{
					x++;
				}
				else if (grid[x][y].direction == down)
				{
					y--;
				}
				else if (grid[x][y].direction == left)
				{
					x--;
				}
			}
		}
		for (int i = 0; i < gridSize; i++)
		{
			for (int j = 0; j < gridSize; j++)
			{
				grid[i][j].finished = false;
				grid[i][j].searched = false;
			}
		}
	}
	if (isEndFound && !isFinished && !oldIsEndFound)
	{
		//std::cout << std::endl << "SCORE AND DIRECTION EVALUATION COMPLETE" << std::endl;
		//std::cout << "BEGINNING PATH FINDING..." << std::endl;
	}
	if (isFinished && !oldIsFinished)
	{
		//std::cout << "PATH-FINDING COMPLETE" << std::endl;
	}
	oldIsFinished = isFinished;
	oldIsEndFound = isEndFound;
}

void Grid::setAsWall(int _x, int _y, bool isWall)
{
	grid[_x][_y].isWall = isWall;
}

void Grid::setStart(int _x, int _y)
{
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			if (grid[i][j].isStart)
			{
				grid[i][j].isStart = false;
			}
		}
	}
	startx = _x;
	starty = _y;
	grid[_x][_y].isStart = true;
	grid[_x][_y].score = 0;
}

bool Grid::setEnd(int _x, int _y)
{
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			if (grid[i][j].isEnd)
			{
				grid[i][j].isEnd = false;
			}
		}
	}
	endx = _x;
	endy = _y;
	grid[_x][_y].isEnd = true;
	return true;
}
