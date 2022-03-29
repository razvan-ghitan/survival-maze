#include "lab_m1/Tema2/Camera.h"

using namespace std;
using namespace m1;


m1::Camera::Camera()
{
    position = glm::vec3(0, 2, 5);
    forward = glm::vec3(0, 0, -1);
    up = glm::vec3(0, 1, 0);
    right = glm::vec3(1, 0, 0);
    distanceToTarget = 2.f;
    globalUp = glm::vec3(0, 1, 0);
    projectionMatrix = glm::mat4(1.f);
}


m1::Camera::~Camera()
{
}


void m1::Camera::Set(const glm::vec3& newPosition, const glm::vec3& center, const glm::vec3& newUp, float distance)
{
    distanceToTarget = distance;
    position = newPosition;
    forward = glm::normalize(center - newPosition);
    right = glm::cross(forward, newUp);
    up = glm::cross(right, forward);
}


void m1::Camera::MoveForward(float distance)
{
    glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    position += dir * distance;
}


void m1::Camera::TranslateForward(float distance)
{
    position += forward * distance;
}


void m1::Camera::TranslateUpward(float distance)
{
    position += up * distance;
}


void m1::Camera::TranslateRight(float distance)
{
    position += right * distance;
}


void m1::Camera::RotateFirstPerson_OX(float angle)
{
    forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1)));
    up = glm::normalize(glm::cross(right, forward));
}


void m1::Camera::RotateFirstPerson_OY(float angle)
{
    forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, globalUp) * glm::vec4(forward, 1)));
    right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, globalUp) * glm::vec4(right, 1)));
    up = glm::cross(right, forward);
}


void m1::Camera::RotateFirstPerson_OZ(float angle)
{
    right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(right, 1)));
    up = glm::normalize(glm::cross(right, forward));
}


void m1::Camera::RotateThirdPerson_OX(float angle)
{
    TranslateForward(distanceToTarget);
    RotateFirstPerson_OX(angle);
    TranslateForward(-distanceToTarget);
}


void m1::Camera::RotateThirdPerson_OY(float angle)
{
    TranslateForward(distanceToTarget);
    RotateFirstPerson_OY(angle);
    TranslateForward(-distanceToTarget);
}


void m1::Camera::RotateThirdPerson_OZ(float angle)
{
    TranslateForward(distanceToTarget);
    RotateFirstPerson_OZ(angle);
    TranslateForward(-distanceToTarget);
}

