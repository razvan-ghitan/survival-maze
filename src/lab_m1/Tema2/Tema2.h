#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/Maze.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {

    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, glm::mat4 matrix, glm::vec3 color, bool readyToDestroy = false);
        void RenderPlayer(float deltaTime);
        void RenderMaze(float deltaTime);

    protected:
        
    private:
        Maze* maze;
        float FOV, speedForFOV;
        float width, height;
        float zNear, zFar;
    };
}   // namespace m1
