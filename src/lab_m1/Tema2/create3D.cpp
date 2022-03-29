#include "lab_m1/Tema2/create3D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Mesh* create3D::GenerateCube(const std::string& name, float length, glm::vec3 color, bool fill)
{
    // Create the vertices
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(0, 0, 0), color, glm::vec3(0)),
        VertexFormat(glm::vec3(length, 0,  0), color, glm::vec3(0)),
        VertexFormat(glm::vec3(0,  0,  length), color, glm::vec3(0)),
        VertexFormat(glm::vec3(length,  0,  length), color, glm::vec3(0)),
        VertexFormat(glm::vec3(0, length, 0), color, glm::vec3(0)),
        VertexFormat(glm::vec3(length, length,  0), color, glm::vec3(0)),
        VertexFormat(glm::vec3(0,  length,  length), color, glm::vec3(0)),
        VertexFormat(glm::vec3(length,  length,  length), color, glm::vec3(0)),
    };

    // Create the indices
    vector<unsigned int> indices =
    {
        0, 1, 2,        1, 3, 2,
        2, 3, 7,        2, 7, 6,
        1, 7, 3,        1, 5, 7,
        6, 7, 4,        7, 5, 4,
        0, 4, 1,        1, 4, 5,
        2, 6, 4,        0, 2, 4,
    };

    // Create the mesh with the given vertices and indices
    return GenerateMesh(name.c_str(), vertices, indices, fill);
}


Mesh* create3D::GenerateRectangularParallel(const std::string& name, float width, float length, float height, glm::vec3 color, bool fill)
{
    // Create the vertices
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(0, 0, 0), color, glm::vec3(0.2, 0.8, 0.2)),
        VertexFormat(glm::vec3(length, 0,  0), color, glm::vec3(0.9, 0.4, 0.2)),
        VertexFormat(glm::vec3(0,  0,  width), color, glm::vec3(0.7, 0.7, 0.1)),
        VertexFormat(glm::vec3(length,  0,  width), color, glm::vec3(0.7, 0.3, 0.7)),
        VertexFormat(glm::vec3(0, height, 0), color, glm::vec3(0.3, 0.5, 0.4)),
        VertexFormat(glm::vec3(length, height,  0), color, glm::vec3(0.5, 0.2, 0.9)),
        VertexFormat(glm::vec3(0,  height,  width), color, glm::vec3(0.7, 0.0, 0.7)),
        VertexFormat(glm::vec3(length,  height,  width), color, glm::vec3(0.1, 0.5, 0.8)),
    };

    // Create the indices
    vector<unsigned int> indices =
    {
        0, 1, 2,        1, 3, 2,
        2, 3, 7,        2, 7, 6,
        1, 7, 3,        1, 5, 7,
        6, 7, 4,        7, 5, 4,
        0, 4, 1,        1, 4, 5,
        2, 6, 4,        0, 2, 4,
    };

    // Create the mesh with the given vertices and indices
    return GenerateMesh(name.c_str(), vertices, indices, fill);
}


Mesh* create3D::GenerateRectangle(const std::string& name, float width, float height, glm::vec3 color, bool fill)
{
    glm::vec3 corner = glm::vec3(0.f);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill)
    {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}


Mesh* create3D::GenerateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices, bool fill)
{
    unsigned int VAO = 0;

    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    Mesh* newMesh = new Mesh(name);
    newMesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    newMesh->vertices = vertices;
    newMesh->indices = indices;

    // Select draw mode
    if (fill)
    {
        newMesh->SetDrawMode(GL_TRIANGLES);
    }
    else
    {
        newMesh->SetDrawMode(GL_LINE_LOOP);
    }

    return newMesh;
}
