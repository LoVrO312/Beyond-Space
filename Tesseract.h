#ifndef TESSERACT_CLASS_H
#define TESSERACT_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>


class Tesseract
{
    public:
        glm::vec4 pointsPositions[16];
        glm::vec3 pointsColors[16];

        Tesseract(); // generates all permutations of 1s and 0s so we get a tesseract where side length a = 1.0
        Tesseract(glm::vec4 points[16]); // might not implement 
        Tesseract(glm::vec4 points[16], glm::vec3 colors[16]); // might not implement anything regarding colors

};
#endif