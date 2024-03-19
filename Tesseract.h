#ifndef TESSERACT_CLASS_H
#define TESSERACT_CLASS_H

#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

class Tesseract
{
    public:
        glm::vec4 pointsPositions[16];
        glm::vec3 pointsColors[16];
        GLfloat projectedVertexAttributes[96];
        
        GLuint wireframeIndices[64]
        {
        // Cube 1
            0, 1,
            0, 2,
            0, 4,	
            5, 1,
            5, 4,
            5, 7,
            6, 2,
            6, 4,
            6, 7,
            3, 1,
            3, 2,
            3, 7,
        // Cube 2
            8, 9,
            8, 10,
            8, 12,	
            13, 9,
            13, 12,
            13, 15,
            14, 10,
            14, 12,
            14, 15,
            11, 9,
            11, 10,
            11, 15,
        // Connecting the two cubes
            0, 8,
            1, 9,
            2, 10,
            3, 11,
            4, 12,
            5, 13,
            6, 14,
            7, 15
        };

        Tesseract(); // generates all permutations of 1s and 0s so we get a tesseract where side length a = 1.0
        Tesseract(glm::vec4 points[16]); // might not implement 
        Tesseract(glm::vec4 points[16], glm::vec3 colors[16]); // might not implement anything regarding colors

        void updateVertexData(VBO& VBO);
        void display();

        // tesseract manipulation functions
        void rotate(float angleDegrees, std::string rotationID);
};
#endif