#include "lab_m1/Tema2/Maze.h"

#include <vector>
#include <iostream>


using namespace m1;


m1::Maze::Maze(int lines, glm::vec3 startCorner)
{
	srand(time(0));

	nrLines = lines;
	exitReached = false;
	cellLength = 10.f;
	startPos = startCorner;
	map = (int**) malloc((nrLines + nrLines) * sizeof(int));
	for (int i = 0; i < nrLines; i++)
	{
		map[i] = (int*) calloc((nrLines + nrLines), sizeof(int));
	}

	CreateMaze();
	ShowMaze();
	CreateCubes();

	// Create the player
	player = new Player(playerPos);
}


m1::Maze::~Maze()
{
}


void m1::Maze::CreateMaze()
{
	int startPozy = rand() % (nrLines - 2) + 1;
	int startPozx = 0;
	map[startPozx][startPozy] = 3;


	int endPozy = rand() % (nrLines - 3) + 1;
	int endPozx = nrLines - 1;
	if (startPozy % 2 == 0 && endPozy % 2 == 1)
	{
		endPozy++;
	}

	map[endPozx][endPozy] = 1;


	int currPozy = rand() % (nrLines - 3) + 1;
	int currPozx = rand() % (nrLines - 2) + 1;
	if (startPozy % 2 == 0 && currPozy % 2 == 0)
	{
		currPozy++;
	}

	SearchPath(currPozx, currPozy, startPozx, startPozy);
	SearchPath(currPozx, currPozy, endPozx, endPozy);

	map[currPozx][currPozy] = 2;

	for (int i = 1; i < nrLines-1; i++)
	{
		for (int j = 1; j < nrLines-1; j++)
		{
			if (rand() % 3 == 0 && !map[i][j])
			{
				map[i][j] = 1;
			}
		}
	}
}


void m1::Maze::SearchPath(int curPozX, int currPozY, int startPozX, int startPozY)
{
	int posStartSearchx = curPozX;
	int posStartSearchy = currPozY;
	int way;
	while (posStartSearchx != startPozX && posStartSearchy != startPozY) 
	{
		way = rand() % 2;
		if (way) 
		{
			if (startPozX > posStartSearchx)
			{
				map[posStartSearchx][posStartSearchy] = 1;
				posStartSearchx++;
			}
			else if (startPozY < posStartSearchx)
			{
				map[posStartSearchx][posStartSearchy] = 1;
				posStartSearchx--;
			}
		}
		else 
		{
			if (startPozY > posStartSearchy)
			{
				map[posStartSearchx][posStartSearchy] = 1;
				posStartSearchy++;
			}
			else if (startPozY < posStartSearchy)
			{
				map[posStartSearchx][posStartSearchy] = 1;
				posStartSearchy--;
			}
		}
	}

	while (posStartSearchx != startPozX) 
	{
		if (startPozX > posStartSearchx)
		{
			map[posStartSearchx][posStartSearchy] = 1;
			posStartSearchx++;
		}
		else if (startPozX < posStartSearchx)
		{
			map[posStartSearchx][posStartSearchy] = 1;
			posStartSearchx--;
		}
	}

	while (posStartSearchy != startPozY)
	{
		if (startPozY > posStartSearchy)
		{
			map[posStartSearchx][posStartSearchy] = 1;
			posStartSearchy++;
		}
		else if (startPozY < posStartSearchy)
		{
			map[posStartSearchx][posStartSearchy] = 1;
			posStartSearchy--;
		}
	}
}


bool m1::Maze::BorderCheck(int x, int y)
{
	if ((x < 0 || x >= nrLines) || (y < 0 || y >= nrLines))
	{
		return false;
	}
	return true;
}


void m1::Maze::CreateCubes()
{
	glm::vec3 currentPos = startPos;
	int currentIndex = 0;
	for (int i = 0; i < nrLines; i++)
	{
		for (int j = 0; j < nrLines; j++)
		{
			// Add wall
			if (map[i][j] == 0)
			{
				Wall* newCell = new Wall;
				newCell->color = glm::vec3(0.247f, 0.f, 0.058f);
				newCell->mesh = create3D::GenerateCube("cellMesh", cellLength, newCell->color, true);
				newCell->location = currentPos;
				newCell->matrix = glm::mat4(1);
				newCell->matrix *= transforms::Translate(newCell->location.x, newCell->location.y, newCell->location.z);
				newCell->matrix *= transforms::Scale(1.f, 1.f, 1.f);

				walls.push_back(newCell);
			}
			// Add floor
			else if (map[i][j] == 1)
			{
				if (i == 0 || i == nrLines - 1 || j == 0 || j == nrLines - 1)
				{
					map[i][j] = 0;
					Wall* newCell = new Wall;
					newCell->color = glm::vec3(0.247f, 0.f, 0.058f);
					newCell->mesh = create3D::GenerateCube("cellMesh", cellLength, newCell->color, true);
					newCell->location = currentPos;
					newCell->matrix = glm::mat4(1);
					newCell->matrix *= transforms::Translate(newCell->location.x, newCell->location.y, newCell->location.z);
					newCell->matrix *= transforms::Scale(1.f, 1.f, 1.f);

					walls.push_back(newCell);
				}
				else
				{
					Wall* newCell = new Wall;
					newCell->color = glm::vec3(0.48f, 0.35f, 0.02f);

					newCell->mesh = create3D::GenerateCube("cellFloor", cellLength, newCell->color, true);
					newCell->location = currentPos;
					newCell->location.y -= cellLength;

					newCell->matrix = glm::mat4(1);
					newCell->matrix *= transforms::Translate(newCell->location.x, newCell->location.y, newCell->location.z);
					newCell->matrix *= transforms::Scale(1.f, 1.f, 1.f);

					walls.push_back(newCell);
					currentIndex++;
				}
			}
			// Add player
			else if (map[i][j] == 2)
			{
				playerPos = currentPos;
				playerPos.x += cellLength / 2;
				playerPos.z += cellLength / 2;

				Wall* newCell = new Wall;
				newCell->color = glm::vec3(0.48f, 0.35f, 0.02f);

				newCell->mesh = create3D::GenerateCube("cellFloor", cellLength, newCell->color, true);
				newCell->location = currentPos;
				newCell->location.y -= cellLength;

				newCell->matrix = glm::mat4(1);
				newCell->matrix *= transforms::Translate(newCell->location.x, newCell->location.y, newCell->location.z);
				newCell->matrix *= transforms::Scale(1.f, 1.f, 1.f);

				walls.push_back(newCell);
				currentIndex++;
			}
			// Add exit
			else if (map[i][j] == 3)
			{
				exitPos = currentPos;
				exitPos.y -= cellLength;

				Wall* newCell = new Wall;
				newCell->color = glm::vec3(0.f, 1.f, 0.f);
				newCell->mesh = create3D::GenerateCube("cellFloor", cellLength, newCell->color, true);
				newCell->location = currentPos;
				newCell->location.y -= cellLength;

				newCell->matrix = glm::mat4(1);
				newCell->matrix *= transforms::Translate(newCell->location.x, newCell->location.y, newCell->location.z);
				newCell->matrix *= transforms::Scale(1.f, 1.f, 1.f);

				walls.push_back(newCell);
			}
			currentPos.x += cellLength;
		}
		currentPos.x = startPos.x;
		currentPos.z += cellLength;
	}
}


bool m1::Maze::CollisionCheck(float minAx, float minAy, float minAz,
								 float maxAx, float maxAy, float maxAz,
								 float minBx, float minBy, float minBz,
								 float maxBx, float maxBy, float maxBz)
{
	return (minAx <= maxBx && maxAx >= minBx) &&
		   (minAy <= maxBy && maxAy >= minBy) &&
		   (minAz <= maxBz && maxAz >= minBz);
}


bool m1::Maze::CheckWallCollisions(glm::vec3 amount)
{
	for (Wall* cell : walls)
	{
		if (strcmp(cell->mesh->GetMeshID(), "cellFloor") != 0)
		{
			player->collisionPosition += amount;

			player->colMinX = player->collisionPosition.x;
			player->colMinY = player->collisionPosition.y;
			player->colMinZ = player->collisionPosition.z;
			player->colMaxX = player->colMinX + 1.5f;
			player->colMaxY = player->colMinY + 2.8f;
			player->colMaxZ = player->colMinZ + 1.3f;

			glm::vec3 cellMax = { cell->location.x + cellLength, cell->location.y + cellLength, cell->location.z + cellLength };
			if (CollisionCheck(player->colMinX, player->colMinY, player->colMinZ, player->colMaxX, player->colMaxY, player->colMaxZ,
							   cell->location.x, cell->location.y, cell->location.z, cellMax.x, cellMax.y, cellMax.z))
			{
				player->collisionPosition -= amount;
				return false;
			}
			player->collisionPosition -= amount;
		}
	}
	return true;
}


void m1::Maze::Update(float deltaTime)
{
	// Check for exit
	for (Wall* cell : walls)
	{
		if (strcmp(cell->mesh->GetMeshID(), "cellFloor") == 0)
		{
			if (cell->location == exitPos)
			{
				glm::vec3 cellMax = { cell->location.x + cellLength, cell->location.y + cellLength, cell->location.z + cellLength };
				if (CollisionCheck(player->colMinX, player->colMinY, player->colMinZ, player->colMaxX, player->colMaxY, player->colMaxZ,
								   cell->location.x, cell->location.y, cell->location.z, cellMax.x, cellMax.y, cellMax.z))
				{
					exitReached = true;
				}
			}
		}
	}
}


void m1::Maze::ShowMaze()
{
	for (int i = 0; i < nrLines; i++)
	{
		for (int j = 0; j < nrLines; j++)
		{
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
}
