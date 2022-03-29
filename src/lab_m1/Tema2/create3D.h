#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace create3D
{
    // Generate the mesh
    Mesh* GenerateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices, bool fill);

    // Create a cube
    Mesh* GenerateCube(const std::string& name, float length, glm::vec3 color, bool fill = false);

    // Create a rectangular parallel
    Mesh* GenerateRectangularParallel(const std::string& name, float width, float length, float height, glm::vec3 color, bool fill = false);

    // Create a rectangle
    Mesh* GenerateRectangle(const std::string& name, float width, float height, glm::vec3 color, bool fill = false);
}
