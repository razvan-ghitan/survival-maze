#include "lab_m1/Tema2/Player.h"

#include <vector>
#include <iostream>


using namespace m1;


m1::Player::Player(glm::vec3 pos)
{
	location = pos;
	rotation = glm::vec3(0.f);
	speed = 4.f;
	isInThirdPerson = true;

	collisionMesh = create3D::GenerateRectangularParallel("playerCollision", 1.3f, 1.5f, 2.8f, glm::vec3(0.f, 1.f, 0.f), false);
	collisionPosition = glm::vec3((2 * pos.x - 1.5f) / 2.f, 0.f, (2 * pos.z - 1.3f) / 2.f);

	camera = new Camera();
	camera->Set(pos + glm::vec3(0.f, 3.f, 5.f), pos + glm::vec3(0.f, 1.4f, 0.f), glm::vec3(0, 1, 0), 5.f);

	thirdPersonCameraLocation = pos + glm::vec3(0.f, 3.f, 3.f);
	thirdPersonCameraCenterLocation = pos + glm::vec3(0.f, 1.4f, 0.f);

	// Create body parts
	Part* part = new Part;
	part->offset = glm::vec3(-0.25f, 2.2f, -0.25f);
	part->color = glm::vec3(0.305f, 0.886f, 0.925f);
	part->mesh = create3D::GenerateCube("head", 0.5f, part->color, true);
	parts.push_back(part);

	part = new Part;
	part->offset = glm::vec3(-0.35f, 1.0f, -0.35f);
	part->color = glm::vec3(0.667f, 0.667f, 0.667f);
	part->mesh = create3D::GenerateRectangularParallel("body", 0.7f, 0.7f, 1.2f, part->color, true);
	parts.push_back(part);

	part = new Part;
	part->offset = glm::vec3(-0.3f, 0.f, -0.1f);
	part->color = glm::vec3(0.615f, 0.f, 1.f);
	part->mesh = create3D::GenerateRectangularParallel("left_leg", 0.2f, 0.2f, 1.f, part->color, true);
	parts.push_back(part);

	part = new Part;
	part->offset = glm::vec3(0.1f, 0.f, -0.1f);
	part->color = glm::vec3(0.615f, 0.f, 1.f);
	part->mesh = create3D::GenerateRectangularParallel("right_leg", 0.2f, 0.2f, 1.f, part->color, true);
	parts.push_back(part);

	part = new Part;
	part->offset = glm::vec3(-0.6f, 1.f, -0.1f);
	part->color = glm::vec3(0.117f, 0.337f, 0.192f);
	part->mesh = create3D::GenerateRectangularParallel("left_hand", 0.2f, 0.2f, 1.1f, part->color, true);
	parts.push_back(part);

	part = new Part;
	part->offset = glm::vec3(0.4f, 1.f, -0.1f);
	part->color = glm::vec3(0.117f, 0.337f, 0.192f);
	part->mesh = create3D::GenerateRectangularParallel("right_hand", 0.2f, 0.2f, 1.1f, part->color, true);
	parts.push_back(part);
}



m1::Player::~Player()
{
}


void m1::Player::Update(float deltaTime)
{
	// Calculate collision matrix
	collisionMatrix = glm::mat4(1);
	collisionMatrix *= transforms::Translate(collisionPosition.x, collisionPosition.y, collisionPosition.z);
	collisionMatrix *= transforms::Scale(1.f, 1.f, 1.f);

	// Calculate collision mins and maxs
	colMinX = collisionPosition.x;
	colMinY = collisionPosition.y;
	colMinZ = collisionPosition.z;

	colMaxX = colMinX + 1.5f;
	colMaxY = colMinY + 2.8f;
	colMaxZ = colMinZ + 1.3f;
}
