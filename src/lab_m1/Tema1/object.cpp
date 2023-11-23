#include "object.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2DD::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2DD::CreateRecangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2DD::CreateDiamondWithCannon(
    const std::string& name,
    glm::vec3 center,
    float size,
    glm::vec3 color)
{
    glm::vec3 top = center + glm::vec3(0, size, 0);
    glm::vec3 rightTopTip = center + glm::vec3(size/2 * 2 / 3, size * 1 / 5, 0);
    glm::vec3 rightTopCorner = center + glm::vec3(size * 1.25, size * 1 / 5, 0);
    glm::vec3 rightBotCorner = center + glm::vec3(size * 1.25, -size * 1 / 5, 0);
    glm::vec3 rightBotTip = center + glm::vec3(size/2 * 2 / 3, -size * 1 / 5, 0);
    glm::vec3 bottom = center + glm::vec3(0, -size, 0);
    glm::vec3 left = center + glm::vec3(-size/2, 0, 0);

    // Calculate cannon vertices
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(top, color),
        VertexFormat(rightTopTip, color),
        VertexFormat(rightTopCorner, color),
        VertexFormat(rightBotCorner, color),
        VertexFormat(rightBotTip, color),
        VertexFormat(bottom, color),
        VertexFormat(left, color),
    };

    Mesh* diamondWithCannon = new Mesh(name);
    std::vector<unsigned int> indices = {0,1,5, 1,4,5, 1,2,4, 0,5,6,  2,3,4};


    diamondWithCannon->InitFromData(vertices, indices);
    return diamondWithCannon;
}


Mesh* object2DD::CreateStar(
    const std::string& name,
    glm::vec3 pos,
    float size,
    glm::vec3 color)
{
    size *= 2;

    glm::vec3 leftTop = pos + glm::vec3(-0.573f * size, 0.184f * size, 0);
    glm::vec3 innerLeftTop = pos + glm::vec3(-0.135f * size, 0.183f * size, 0);
    glm::vec3 top = pos + glm::vec3(0, 0.6f * size, 0);
    glm::vec3 innerRightTop = pos + glm::vec3(0.134f * size, 0.183f * size, 0);
    glm::vec3 rightTop = pos + glm::vec3(0.572f * size, 0.182f * size, 0);


    glm::vec3 leftBot = pos + glm::vec3(-0.355f * size, -0.490f * size, 0);
    glm::vec3 rightBot = pos + glm::vec3(0.353f * size,  -0.490f * size, 0);
    glm::vec3 innerLeftBot = pos + glm::vec3(-0.219f * size, -0.073f * size, 0);

    glm::vec3 innerRightBot = pos + glm::vec3(0.218 * size, -0.074 * size, 0);

    glm::vec3 inner = pos + glm::vec3(0, -0.233f * size, 0);

    glm::vec3 center = pos + glm::vec3(0,0,0);

   


    std::vector<VertexFormat> vertices =
    {
        VertexFormat(leftTop, color),
        VertexFormat(innerLeftTop, color),
        VertexFormat(top, color),
        VertexFormat(innerRightTop, color),
        VertexFormat(rightTop, color),
        VertexFormat(leftBot, color),
        VertexFormat(rightBot, color),
        VertexFormat(innerLeftBot, color),
        VertexFormat(innerRightBot, color),
        VertexFormat(inner, color),
        VertexFormat(center, color),

    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices = {10, 2, 1,   0, 7, 5,   9, 6, 8,  4, 3, 2};

    star->SetDrawMode(GL_TRIANGLE_FAN);
    star->InitFromData(vertices, indices);
    return star;
}

Mesh* object2DD::CreateHexagon(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int numVertices = 6; // Hexagon has 6 vertices

    for (int i = 0; i < numVertices; ++i) {
        float angle = static_cast<float>(i) * 2.0f * glm::pi<float>() / numVertices;
        glm::vec3 vertexPosition(center.x + radius * cos(angle),
            center.y + radius * sin(angle),
            center.z);
        vertices.push_back(VertexFormat(vertexPosition, color));

        indices.push_back(i);
    }
    Mesh* hexagon = new Mesh(name);

    // If fill is true, triangulate the hexagon
    if (fill) {
        for (int i = 1; i <= numVertices - 2; ++i) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        indices.push_back(0);
        indices.push_back(numVertices - 1);
        indices.push_back(1);
    }
    else {
        // Set draw mode to wireframe if fill is false
        hexagon->SetDrawMode(GL_LINE_LOOP);
    }

    hexagon->InitFromData(vertices, indices);
    return hexagon;
}




