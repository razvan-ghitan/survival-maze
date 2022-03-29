#pragma once

#include <glm/glm.hpp>
#include "core/gpu/mesh.h"
#include "lab_m1/Tema2/create3D.h"
#include "lab_m1/Tema2/transforms.h"
#include "lab_m1/Tema2/Player.h"

using namespace std;

namespace m1
{
	struct Wall
	{
		Mesh* mesh;
		glm::vec3 location;
		glm::vec3 color;
		glm::mat4 matrix;
	};

    class Maze
    {

    public:
        Maze(int lines, glm::vec3 startCorner);
        ~Maze();

    private:
		void CreateMaze();
		void SearchPath(int curPozX, int currPozY, int startPozX, int startPozY);
		bool BorderCheck(int x, int y);
		void CreateCubes();
		bool CollisionCheck(float minAx, float minAy, float minAz, 
							float maxAx, float maxAy, float maxAz,
							float minBx, float minBy, float minBz,
							float maxBx, float maxBy, float maxBz);

    public:
		int nrLines;
		float cellLength;
		int** map;
		bool exitReached;
		glm::vec3 startPos;
		glm::vec3 playerPos;
		glm::vec3 exitPos;
		std::vector<Wall*> walls;
		Player* player;

		void Update(float deltaTime);
		bool CheckWallCollisions(glm::vec3 amount);
		void ShowMaze();
    };
}   // namespace m1
