#include "Tesseract.h"
#include "iostream"

Tesseract::Tesseract()
{   // generates all permutations of vec4 containing 1s and 0s so we get a tesseract where side length a = 1.0
            int t = 0;
            for (int i = 0; i <= 1; ++i)
                for (int j = 0; j <= 1; ++j)
                    for (int k = 0; k <= 1; ++k)
                        for (int l = 0; l <= 1; ++l)
                        {
                            this->pointsPositions[t++] = glm::vec4((float)i, (float)j, (float)k, (float)l);
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

void Tesseract::display()
{
    for (int i = 0; i < 16; i++)
    {
        // moram projicirati tocku u 3d i onda nju i njenu boju dodati u projectedVertexAttributes
        // kako bi to mogao poslati VAOu i VBOu
        // mozda napraviti novi VBO za svaki a da dijele EBO i VAO kako bi mogao puno njih spawnati u isto vrijeme
        // dinamicki ih alocirati pomocu tipke i onda deletati kad stisnemo delete il nes
    }

}