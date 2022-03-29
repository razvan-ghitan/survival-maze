#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <iostream>

namespace m1
{
    class Camera
    {

    public:
        Camera();
        ~Camera();

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec3 globalUp;
        glm::mat4 projectionMatrix;
        glm::mat4 GetViewMatrix() { return glm::lookAt(position, position + forward, up); };
        glm::vec3 GetTargetPosition() { return position + forward * distanceToTarget; }

        void Set(const glm::vec3& newPosition, const glm::vec3& center, const glm::vec3& newUp, float distance);

        void MoveForward(float distance);
        void TranslateForward(float distance);
        void TranslateUpward(float distance);
        void TranslateRight(float distance);

        void RotateFirstPerson_OX(float angle);
        void RotateFirstPerson_OY(float angle);
        void RotateFirstPerson_OZ(float angle);

        void RotateThirdPerson_OX(float angle);
        void RotateThirdPerson_OY(float angle);
        void RotateThirdPerson_OZ(float angle);
    };
}   // namespace m1
