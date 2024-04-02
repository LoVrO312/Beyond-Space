#include "Tesseract.h"
#include "iostream"

#include <cstdlib>
#include <ctime>

Tesseract::Tesseract(float posX, float posY, float posZ)
{
    worldPosition = glm::vec3(posX, posY, posZ);
    
    for (int i = 0; i < 8; i++)
    {
        pointsPositions[i] = glm::vec4((i & 1) ? 1.0f : -1.0f, (i & 2) ? 1.0f : -1.0f, (i & 4) ? 1.0f : -1.0f, 0.0f);
    }
    for (int i = 8; i < 16; i++)
    {
        pointsPositions[i] = glm::vec4((i & 1) ? 1.0f : -1.0f, (i & 2) ? 1.0f : -1.0f, (i & 4) ? 1.0f : -1.0f, -1.0f);
    }

    srand(time(nullptr));

    // Generate random colors for each vertex
    for (int i = 0; i < 16; i++)
    {
        float r = static_cast<float>(rand()) / RAND_MAX;
        float g = static_cast<float>(rand()) / RAND_MAX;
        float b = static_cast<float>(rand()) / RAND_MAX;
        pointsColors[i] = glm::vec3(r, g, b);
    }


    // VAO VBO EBO linking and initialization:
	VAO_tes.Bind();
	updateVertexData(VBO_tes);

    EBO_tes = new EBO(wireframeIndices, sizeof(wireframeIndices));

	VAO_tes.LinkAttrib(VBO_tes, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_tes.LinkAttrib(VBO_tes, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO_tes.Unbind();
	EBO_tes->Unbind();
    
}

Tesseract::Tesseract(glm::vec4 points[16])
{   
    for(int i = 0; i <= 15; i++)
    {
        pointsPositions[i] = points[i];
        pointsColors[i] = glm::vec3(1.0f, 1.0f, 1.0f);
    }
}

Tesseract* Tesseract::createNewTesseract(float posX, float posY, float posZ)
{
    Tesseract* tesseract = new Tesseract(posX, posY, posZ);
    tesseract->draw();
    return tesseract; 
}

void Tesseract::updateVertexData(VBO& VBO)
{
    float distance = 2;
    float k; 
    for (int i = 0, j = 0; i < 16; i++, j += 6)
    {
        k = 1 / (distance - pointsPositions[i].w);

        // perspective projection of point into 3D and adding position offset from origin
        projectedVertexAttributes[j]     = pointsPositions[i].x * k + worldPosition.x;
        projectedVertexAttributes[j + 1] = pointsPositions[i].y * k + worldPosition.y;
        projectedVertexAttributes[j + 2] = pointsPositions[i].z * k + worldPosition.z;

        // giving point colors
        projectedVertexAttributes[j + 3] = pointsColors[i].x;
        projectedVertexAttributes[j + 4] = pointsColors[i].y;
        projectedVertexAttributes[j + 5] = pointsColors[i].z;
    
    }
    VBO.Update(projectedVertexAttributes, sizeof(projectedVertexAttributes), GL_DYNAMIC_DRAW);

}

void Tesseract::draw()
{
    updateVertexData(VBO_tes);
	VAO_tes.Bind();
    glDrawElements(GL_LINES, sizeof(wireframeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

void Tesseract::rotate(float angleDegrees, std::string rotationID)
{
    float angleRadians = glm::radians(angleDegrees);
    float sin = glm::sin(angleRadians);
    float cos = glm::cos(angleRadians);

    glm::vec4 matrix[4];
    if (rotationID == "xw" || rotationID == "wx")
    {
        matrix[0] = glm::vec4(1,  0,    0, 0);
        matrix[1] = glm::vec4(0,  cos,  -sin, 0);
        matrix[2] = glm::vec4(0,  sin,  cos, 0);
        matrix[3] = glm::vec4(0,  0,    0, 1);
    }
    else if (rotationID == "yw" || rotationID == "wy")
    {
        matrix[0] = glm::vec4(cos,  0,  -sin, 0);
        matrix[1] = glm::vec4(0,    1,   0, 0);
        matrix[2] = glm::vec4(sin, 0,  cos, 0);
        matrix[3] = glm::vec4(0,    0,   0, 1);
    }
    else if (rotationID == "zw" || rotationID == "wz")
    {
        matrix[0] = glm::vec4(cos,  -sin, 0, 0);
        matrix[1] = glm::vec4(sin,  cos,  0, 0);
        matrix[2] = glm::vec4(0,     0,   1, 0);
        matrix[3] = glm::vec4(0,     0,   0, 1);
    }
    else if (rotationID == "xy" || rotationID == "yx")
    {
        matrix[0] = glm::vec4(1,  0, 0,   0);
        matrix[1] = glm::vec4(0,  1, 0,   0);
        matrix[2] = glm::vec4(0,  0, cos, -sin);
        matrix[3] = glm::vec4(0,  0, sin, cos);
    }
    else if (rotationID == "yz" || rotationID == "zy")
    {
        matrix[0] = glm::vec4(cos, 0,  0, -sin);
        matrix[1] = glm::vec4(0,   1,  0,  0);
        matrix[2] = glm::vec4(0,   0,  1,  0);
        matrix[3] = glm::vec4(sin, 0,  0,  cos);
    }
    else if (rotationID == "xz" || rotationID == "zx")
    {
        matrix[0] = glm::vec4(1,  0,  0,   0);
        matrix[1] = glm::vec4(0,  cos, 0, -sin);
        matrix[2] = glm::vec4(0,  0,   1,  0);
        matrix[3] = glm::vec4(0,  sin, 0,  cos);
    }
    else return;
    
    glm::vec3 colmatrix[3];
    colmatrix[0] = glm::vec3(cos,  -sin, 0);
    colmatrix[1] = glm::vec3(sin,  cos,  0);
    colmatrix[2] = glm::vec3(0,     0,   1);

    for (int i = 0; i < 16; i++)
    {   
        // rotating the points
        glm::vec4 vec = pointsPositions[i];
        pointsPositions[i].x = vec.x * matrix[0].x + vec.y * matrix[0].y + vec.z * matrix[0].z + vec.w * matrix[0].w;
        pointsPositions[i].y = vec.x * matrix[1].x + vec.y * matrix[1].y + vec.z * matrix[1].z + vec.w * matrix[1].w;
        pointsPositions[i].z = vec.x * matrix[2].x + vec.y * matrix[2].y + vec.z * matrix[2].z + vec.w * matrix[2].w;
        pointsPositions[i].w = vec.x * matrix[3].x + vec.y * matrix[3].y + vec.z * matrix[3].z + vec.w * matrix[3].w;

        // assigning new colors to the points based on a 3d rotation matrix
        glm::vec3 col = pointsColors[i];
        pointsColors[i].x = glm::dot(colmatrix[0], col);
        pointsColors[i].y = glm::dot(colmatrix[1], col);
        pointsColors[i].z = glm::dot(colmatrix[2], col);
    }

}

