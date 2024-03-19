#include "Cube.h"

Cube::Cube()
{
    GLfloat defaultPositions[24] = 
	{ 
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
	    -0.5f, -0.5f, -0.5f,
	};
    GLfloat defaultColors[24];
	std::fill(defaultColors, defaultColors + 24, 1.0);

    position = glm::vec3(0.0f, 0.0f, 0.0f);

    setPositions(defaultPositions);
    setColors(defaultColors);
}

void Cube::setPositions(const GLfloat* positions)
{
    std::copy(positions, positions + 24, vertexPositions);
}

void Cube::setColors(const GLfloat* colors)
{
    std::copy(colors, colors + 24, vertexColors);
}

void Cube::updateVertexData(VBO& VBO)
{
    // Combine positions and colors into a single array
    for (int i = 0, j = 0; i < 48; i += 6, j += 3)
    {
        std::copy(vertexPositions + j, vertexPositions + j + 3, combinedData + i);
        std::copy(vertexColors + j, vertexColors + j + 3, combinedData + i + 3);
    }

    VBO.Update(combinedData, sizeof(combinedData), GL_DYNAMIC_DRAW);
}

void Cube::translate(float x, float y, float z)
{
    position = glm::vec3(x, y, z);
    
    for (int i = 0; i < 24; i += 3)
    {
        vertexPositions[i]   += x;
        vertexPositions[i+1] += y;
        vertexPositions[i+2] += z;
    }
}

void Cube::rotate(float angleDegrees, char rotationID)
{
    float angleRadians = glm::radians(angleDegrees);
    float sin = glm::sin(angleRadians);
    float cos = glm::cos(angleRadians);

    glm::vec3 matrix[3];
    if (rotationID == 'x')
    {
        matrix[0] = glm::vec3(1,  0,    0);
        matrix[1] = glm::vec3(0,  cos,  -sin);
        matrix[2] = glm::vec3(0,  sin,  cos);
    }
    else if (rotationID == 'y')
    {
        matrix[0] = glm::vec3(cos,  0,  sin);
        matrix[1] = glm::vec3(0,    1,   0);
        matrix[2] = glm::vec3(-sin, 0,  cos);
    }
    else if (rotationID == 'z')
    {
        matrix[0] = glm::vec3(cos,  -sin, 0);
        matrix[1] = glm::vec3(sin,  cos,  0);
        matrix[2] = glm::vec3(0,     0,   1);
    }
    else return;

    for (int i = 0; i < 24; i += 3)
    {
        float x = vertexPositions[i] - position.x;
        float y = vertexPositions[i+1] - position.y;
        float z = vertexPositions[i+2] - position.z;

        glm::vec3 vec = glm::vec3(x, y, z);

        vertexPositions[i]   = glm::dot(matrix[0], vec) + position.x;
        vertexPositions[i+1] = glm::dot(matrix[1], vec) + position.y;
        vertexPositions[i+2] = glm::dot(matrix[2], vec) + position.z;
    }
}