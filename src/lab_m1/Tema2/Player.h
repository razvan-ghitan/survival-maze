#pragma once

#include "lab_m1/Tema2/Camera.h"
#include "lab_m1/Tema2/create3D.h"
#include "lab_m1/Tema2/transforms.h"

using namespace std;

namespace m1
{
    struct Part
    {
        Mesh* mesh;
        glm::vec3 offset;
        glm::vec3 color;
    };

    class Player
    {

    public:
        Player(glm::vec3 pos);
        ~Player();
      
    public:
        Camera* camera;
        glm::vec3 location;
        glm::vec3 rotation;
        float speed;
        bool isInThirdPerson;

        glm::vec3 thirdPersonCameraLocation;
        glm::vec3 thirdPersonCameraCenterLocation;

        Mesh* collisionMesh;
        glm::vec3 collisionPosition;
        glm::mat4 collisionMatrix;

        std::vector<Part*> parts;
        float colMaxX, colMaxY, colMaxZ;
        float colMinX, colMinY, colMinZ;

        void Update(float DeltaTime);
    };
}   // namespace m1
