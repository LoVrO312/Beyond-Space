#ifndef CUBE_H
#define CUBE_H

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<libraries/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

class Cube
{
public:
    GLfloat vertexPositions[24];
    GLfloat vertexColors[24];
	GLfloat combinedData[48];
	GLuint  wireframeIndices[24] = 
	{
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
		3, 7
	};

    Cube();

    void setPositions(const GLfloat* positions);
    void setColors(const GLfloat* colors);
    void updateVertexData(VBO& VBO);

	// cube manipulation functions
	void translate(glm::vec3 newPosition);
    void rotate(float angleRadians, char rotationID);
};

#endif