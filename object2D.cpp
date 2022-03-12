#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2Dtema1::CreateSquare(
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

Mesh* object2Dtema1::CreateHealthbarSquare(
    const std::string& name,
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
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2Dtema1::CreatePlayer(
    const std::string& name,
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
        VertexFormat(corner + glm::vec3(0, length, 0), color),
        
        VertexFormat(corner + glm::vec3(length/2, length, 0), glm::vec3(1,0,0)),
        VertexFormat(corner + glm::vec3(length, length/2, 0), glm::vec3(0,1,0)),
        VertexFormat(corner + glm::vec3(length/2, 0, 0), glm::vec3(0,0,1))
        /*
        VertexFormat(corner + glm::vec3(0, length/2, 0), glm::vec3(0,0,1)),
        VertexFormat(corner + glm::vec3(length/2, 0, 0), glm::vec3(0,0,1)),
        VertexFormat(corner + glm::vec3(length, length/2, 0), glm::vec3(0,0,1))
        */
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 6,4,5,0,1,2,2,3,0 };

    //if (!fill) {
        square->SetDrawMode(GL_TRIANGLES);
    //}
    //else {
        // Draw 2 triangles. Add the remaining 2 indices
        //indices.push_back(0);
        //indices.push_back(2);
    //}

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2Dtema1::CreateEnemy(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner+glm::vec3(length/2, length/2, length/2), glm::vec3(1)),
        VertexFormat(corner + glm::vec3(length, 0, 0), glm::vec3(1)),
        VertexFormat(corner + glm::vec3(length, length, 0), glm::vec3(1)),
        VertexFormat(corner + glm::vec3(0, length, 0), glm::vec3(1)),
        
        
        /*
        VertexFormat(corner + glm::vec3(0, length/2, 0), glm::vec3(0,0,1)),
        VertexFormat(corner + glm::vec3(length/2, 0, 0), glm::vec3(0,0,1)),
        VertexFormat(corner + glm::vec3(length, length/2, 0), glm::vec3(0,0,1))
        */
    };
    for (int i = 0; i < 360; i++) {
        vertices.push_back(VertexFormat(glm::vec3(length*cos(i), length*sin(i), 0), color));
    }
    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0,1,2,2,3,0 };
    for (int i = 1; i < 360; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i+1);
    }
    //if (!fill) {
    square->SetDrawMode(GL_TRIANGLES);
    //}
    //else {
        // Draw 2 triangles. Add the remaining 2 indices
        //indices.push_back(0);
        //indices.push_back(2);
    //}

    square->InitFromData(vertices, indices);
    return square;
}


