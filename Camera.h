#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"
#include"Tesseract.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	float FOVdeg;
	bool previousTKeyState = false;
	bool previousRKeyState = false;
	bool rotateTesseracts = true;
	bool rotation[6] = { false, false, false, false, false, false };
	bool previousKeyStates[6] = { false, false, false, false, false, false };

	// we can access dynamically allocated tesseract objects here 
	std::vector<Tesseract*> tesseracts;

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 0.001f;
	float sensitivity = 100.0f;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position, float fovDeg);

	void draw();

	// Updates and exports the camera matrix to the Vertex Shader
	void Matrix(float nearPlane, float farPlane, Shader& shader, const char* uniform);
	// Handles camera inputs
	void InputsAndDraws(GLFWwindow* window);
};
#endif