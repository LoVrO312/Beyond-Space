#include "Tesseract.h"
#include "iostream"

#include <cstdlib>
#include <ctime>

Tesseract::Tesseract()
{   // generates all permutations of vec4 containing 1s and 0s so we get a tesseract where side length a = 1.0
        
    int t = 0;
    for (int i = -1; i <= 1; i += 2)
        for (int j = -1; j <= 1; j += 2)
            for (int k = -1; k <= 1; k += 2)
                for (int l = -1; l <= 1; l += 2)
                {
                    pointsPositions[t++] = glm::vec4((float)l, (float)k, (float)j, (float)i);
                }
    
    for (int i = 0; i < 8; i++)
    {
        pointsPositions[i] = glm::vec4((i & 1) ? 1.0f : -1.0f, (i & 2) ? 1.0f : -1.0f, (i & 4) ? 1.0f : -1.0f, 0.0f);
    }
    for (int i = 8; i < 16; i++)
    {
        pointsPositions[i] = glm::vec4((i & 1) ? 1.0f : -1.0f, (i & 2) ? 1.0f : -1.0f, (i & 4) ? 1.0f : -1.0f, -1.0f);
    }

    // ! Original code
    // for (int i = 0; i < 16; i++)
    //     pointsColors[i] = glm::vec3(1.0f, 1.0f, 1.0f);

    srand(time(nullptr));

    // Generate random colors for each vertex
    for (int i = 0; i < 16; i++)
    {
        float r = static_cast<float>(rand()) / RAND_MAX; // Random value between 0 and 1 for red component
        float g = static_cast<float>(rand()) / RAND_MAX; // Random value between 0 and 1 for green component
        float b = static_cast<float>(rand()) / RAND_MAX; // Random value between 0 and 1 for blue component
        pointsColors[i] = glm::vec3(r, g, b);
    }
}

Tesseract::Tesseract(glm::vec4 points[16])
{   
    for(int i = 0; i <= 15; i++)
    {
        this->pointsPositions[i] = points[i];
        this->pointsColors[i] = glm::vec3(1.0f, 1.0f, 1.0f);
    }
}

Tesseract::Tesseract(glm::vec4 points[16], glm::vec3 colors[16])
{
    for(int i = 0; i <= 15; i++)
    {
        this->pointsPositions[i] = points[i];
        this->pointsColors[i] = colors[i];
    }
}


// TODO mozda napraviti novi VBO za svaki a da dijele EBO i VAO kako bi mogao puno njih spawnati u isto vrijeme
// TODO dinamicki ih alocirati pomocu tipke i onda deletati kad stisnemo delete il nes
void Tesseract::updateVertexData(VBO& VBO)
{
    float distance = 2;
    float k; 
    for (int i = 0, j = 0; i < 16; i++, j += 6)
    {
        k = 1 / (distance - pointsPositions[i].w);

        // perspective projection of point into 3D
        projectedVertexAttributes[j]     = pointsPositions[i].x * k;
        projectedVertexAttributes[j + 1] = pointsPositions[i].y * k;
        projectedVertexAttributes[j + 2] = pointsPositions[i].z * k;

        // giving point colors
        projectedVertexAttributes[j + 3] = pointsColors[i].x;
        projectedVertexAttributes[j + 4] = pointsColors[i].y;
        projectedVertexAttributes[j + 5] = pointsColors[i].z;
    
    }
    VBO.Update(projectedVertexAttributes, sizeof(projectedVertexAttributes), GL_DYNAMIC_DRAW);

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
        matrix[0] = glm::vec4(cos,  0,  sin, 0);
        matrix[1] = glm::vec4(0,    1,   0, 0);
        matrix[2] = glm::vec4(-sin, 0,  cos, 0);
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