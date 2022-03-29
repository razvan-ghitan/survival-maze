#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <iostream>


using namespace std;
using namespace m1;


m1::Tema2::Tema2()
{
    // Camera variables
    FOV = RADIANS(60.0f);
    speedForFOV = 5;
    width = 5;
    height = 5;
    zNear = 0.01f;
    zFar = 200.0f;

    // Create the maze
    maze = new Maze(20, glm::vec3(0.f));
    maze->player->camera->projectionMatrix = glm::perspective(FOV, window->props.aspectRatio, zNear, zFar);

    Mesh* mesh = new Mesh("sphere");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[mesh->GetMeshID()] = mesh;
}


m1::Tema2::~Tema2()
{
}


void m1::Tema2::Init()
{
    // Create the shader
    Shader* shader = new Shader("ModelShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "Shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "Shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}


void m1::Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.f, 0.f, 0.f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void m1::Tema2::Update(float deltaTimeSeconds)
{
    if (maze->exitReached)
    {
        printf("You won!\n");
    }
    else
    {
        RenderPlayer(deltaTimeSeconds);
        RenderMaze(deltaTimeSeconds);

        if (!maze->player->isInThirdPerson)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transforms::Translate(maze->player->camera->GetTargetPosition().x,
                                                maze->player->camera->GetTargetPosition().y,
                                                maze->player->camera->GetTargetPosition().z);
            modelMatrix *= transforms::Scale(0.1f, 0.1f, 0.1f);
            RenderSimpleMesh(meshes["sphere"], shaders["ModelShader"], modelMatrix, glm::vec3(0.223f, 1.f, 0.078f));
        }
    }
}


void m1::Tema2::FrameEnd()
{
}


void m1::Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Move forward
    if (window->KeyHold(GLFW_KEY_W) || window->KeyHold(GLFW_KEY_UP))
    {
        glm::vec3 direction = glm::normalize(glm::vec3(maze->player->camera->forward.x, 0, maze->player->camera->forward.z));
        
        if (maze->CheckWallCollisions(direction * deltaTime * maze->player->speed))
        {
            maze->player->location = maze->player->location + direction * deltaTime * maze->player->speed;
            maze->player->collisionPosition = maze->player->collisionPosition + direction * deltaTime * maze->player->speed;
            maze->player->camera->MoveForward(deltaTime * maze->player->speed);

            glm::vec3 tpCamPos = maze->player->thirdPersonCameraLocation;
            glm::vec3 tpCamCenterPos = maze->player->thirdPersonCameraCenterLocation;

            tpCamPos += direction * deltaTime * maze->player->speed;
            tpCamCenterPos += direction * deltaTime * maze->player->speed;

            maze->player->thirdPersonCameraLocation = tpCamPos;
            maze->player->thirdPersonCameraCenterLocation = tpCamCenterPos;
        }
    }

    // Move backwards
    if (window->KeyHold(GLFW_KEY_S) || window->KeyHold(GLFW_KEY_DOWN))
    {
        glm::vec3 direction = glm::normalize(glm::vec3(maze->player->camera->forward.x, 0, maze->player->camera->forward.z));

        if (maze->CheckWallCollisions(-direction * deltaTime * maze->player->speed))
        {
            maze->player->location = maze->player->location - direction * deltaTime * maze->player->speed;
            maze->player->collisionPosition = maze->player->collisionPosition - direction * deltaTime * maze->player->speed;
            maze->player->camera->MoveForward(-deltaTime * maze->player->speed);

            glm::vec3 tpCamPos = maze->player->thirdPersonCameraLocation;
            glm::vec3 tpCamCenterPos = maze->player->thirdPersonCameraCenterLocation;

            tpCamPos -= direction * deltaTime * maze->player->speed;
            tpCamCenterPos -= direction * deltaTime * maze->player->speed;

            maze->player->thirdPersonCameraLocation = tpCamPos;
            maze->player->thirdPersonCameraCenterLocation = tpCamCenterPos;
        }
    }

    // Move right
    if (window->KeyHold(GLFW_KEY_D) || window->KeyHold(GLFW_KEY_RIGHT))
    {
        glm::vec3 direction = maze->player->camera->right;

        if (maze->CheckWallCollisions(direction * deltaTime * maze->player->speed))
        {
            maze->player->location = maze->player->location + direction * deltaTime * maze->player->speed;
            maze->player->collisionPosition = maze->player->collisionPosition + direction * deltaTime * maze->player->speed;
            maze->player->camera->TranslateRight(deltaTime * maze->player->speed);

            glm::vec3 tpCamPos = maze->player->thirdPersonCameraLocation;
            glm::vec3 tpCamCenterPos = maze->player->thirdPersonCameraCenterLocation;

            tpCamPos += direction * deltaTime * maze->player->speed;
            tpCamCenterPos += direction * deltaTime * maze->player->speed;

            maze->player->thirdPersonCameraLocation = tpCamPos;
            maze->player->thirdPersonCameraCenterLocation = tpCamCenterPos;
        }
    }

    // Move left
    if (window->KeyHold(GLFW_KEY_A) || window->KeyHold(GLFW_KEY_LEFT))
    {
        glm::vec3 direction = maze->player->camera->right;

        if (maze->CheckWallCollisions(-direction * deltaTime * maze->player->speed))
        {
            maze->player->location = maze->player->location - direction * deltaTime * maze->player->speed;
            maze->player->collisionPosition = maze->player->collisionPosition - direction * deltaTime * maze->player->speed;
            maze->player->camera->TranslateRight(-deltaTime * maze->player->speed);

            glm::vec3 tpCamPos = maze->player->thirdPersonCameraLocation;
            glm::vec3 tpCamCenterPos = maze->player->thirdPersonCameraCenterLocation;

            tpCamPos -= direction * deltaTime * maze->player->speed;
            tpCamCenterPos -= direction * deltaTime * maze->player->speed;

            maze->player->thirdPersonCameraLocation = tpCamPos;
            maze->player->thirdPersonCameraCenterLocation = tpCamCenterPos;
        }
    }
}


void m1::Tema2::OnKeyPress(int key, int mods)
{
    // Switch to the other camera
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
    {
        if (maze->player->isInThirdPerson == true)
        {
            maze->player->isInThirdPerson = false;

            maze->player->thirdPersonCameraLocation = maze->player->camera->position;
            maze->player->thirdPersonCameraCenterLocation = maze->player->camera->GetTargetPosition();

            glm::vec3 forwardDir = glm::normalize(glm::vec3(maze->player->camera->forward.x, 0.f, maze->player->camera->forward.z));
            glm::vec3 fpCamLoc = maze->player->location + forwardDir;
            fpCamLoc = glm::vec3(fpCamLoc.x, 2.f, fpCamLoc.z);
            
            maze->player->camera->Set(fpCamLoc, fpCamLoc + forwardDir, glm::vec3(0, 1, 0), 2.f);
        }
        else
        {
            maze->player->isInThirdPerson = true;
            maze->player->rotation = glm::vec3(0.f);
            maze->player->camera->Set(maze->player->thirdPersonCameraLocation, maze->player->thirdPersonCameraCenterLocation, glm::vec3(0, 1, 0), 5.f);
        }
    }
}


void m1::Tema2::OnKeyRelease(int key, int mods)
{
}


void m1::Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensOX = 0.008f;
    float sensOY = 0.008f;
    if (window->GetSpecialKeyState() == 0)
    {
        if (maze->player->isInThirdPerson)
        {
            maze->player->camera->RotateThirdPerson_OY(-sensOY * deltaX);
        }
        else
        {
            maze->player->camera->RotateFirstPerson_OY(-sensOY * deltaX);

            glm::vec3 playerRotation = maze->player->rotation;
            playerRotation.y += sensOY * deltaX;
            maze->player->rotation = playerRotation;
        }
    }
}


void m1::Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Switch to the other camera
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (maze->player->isInThirdPerson == true)
        {
            maze->player->isInThirdPerson = false;
            maze->player->thirdPersonCameraLocation =maze->player->camera->position;
            maze->player->thirdPersonCameraCenterLocation = maze->player->camera->GetTargetPosition();

            glm::vec3 dir = glm::normalize(glm::vec3(maze->player->camera->forward.x, 0.f, maze->player->camera->forward.z));
            glm::vec3 fpCamLoc = maze->player->location + dir;
            fpCamLoc = glm::vec3(fpCamLoc.x, 2.f, fpCamLoc.z);
             
            maze->player->camera->Set(fpCamLoc, fpCamLoc + dir, glm::vec3(0, 1, 0), 2.f);
        }
        else
        {
            maze->player->isInThirdPerson = true;
            maze->player->rotation = glm::vec3(0.f);
            maze->player->camera->Set(maze->player->thirdPersonCameraLocation,
                                                maze->player->thirdPersonCameraCenterLocation,
                                                glm::vec3(0, 1, 0), 5.f);
        }
    }
}


void m1::Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void m1::Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void m1::Tema2::OnWindowResize(int width, int height)
{
}


void m1::Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, glm::mat4 matrix, glm::vec3 color, bool readyToDestroy)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model color
    glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "object_color"), 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(matrix));

    // Bind view matrix
    glm::mat4 viewMatrix = maze->player->camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = maze->player->camera->projectionMatrix;
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Bind destroyable status
    glUniform1i(glGetUniformLocation(shader->GetProgramID(), "displacementFlag"), (int)readyToDestroy);

    // Bind time
    float applicationTime = Engine::GetElapsedTime();
    glUniform1f(glGetUniformLocation(shader->GetProgramID(), "time"), applicationTime);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void m1::Tema2::RenderPlayer(float deltaTime)
{
    // Update and render the player
    maze->player->Update(deltaTime);
    if (maze->player->isInThirdPerson)
    {
        for (Part* limb : maze->player->parts)
        {
            glm::mat4 matrix = glm::mat4(1);
            matrix *= transforms::Translate(maze->player->location.x + limb->offset.x,
                                            maze->player->location.y + limb->offset.y,
                                            maze->player->location.z + limb->offset.z);
            matrix *= transforms::Scale(1.f, 1.f, 1.f);
            RenderSimpleMesh(limb->mesh, shaders["ModelShader"], matrix, limb->color);
        }
    }
}


void m1::Tema2::RenderMaze(float deltaTime)
{
    // Render labyrinth
    maze->Update(deltaTime);
    for (Wall* cell : maze->walls)
    {
        RenderSimpleMesh(cell->mesh, shaders["ModelShader"], cell->matrix, cell->color);
    }
}
