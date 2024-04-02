#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 position, float FOVdeg)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
	this->FOVdeg = FOVdeg;
}

void Camera::Matrix(float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::InputsAndDraws(GLFWwindow* window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	// speed up with left shift
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.04f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	// reset camera position to start
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		Position = glm::vec3(0.0f, 0.0f, 4.0f);
	}
	// Change FOV
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		FOVdeg += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		FOVdeg -= 0.1f;
	}

	// spawns tesseract ~2 meters in front of camera
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !previousTKeyState)
    {
		glm::vec3 tesseractPosition = Position + 2.0f * Orientation;
        Tesseract* tesseract = Tesseract::createNewTesseract(tesseractPosition.x, tesseractPosition.y, tesseractPosition.z);
		tesseracts.push_back(tesseract);
    }
    previousTKeyState = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;

	// toggle tesseracts rotation
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !previousRKeyState)
    {
		rotateTesseracts = !rotateTesseracts;
    }
    previousRKeyState = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
	
	if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
	{
		for (auto tess : tesseracts)
			delete tess;

		tesseracts.clear();
	}

	// rotation toggling
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !previousKeyStates[0])
    {
		rotation[0] = !rotation[0];
    }
	previousKeyStates[0] = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !previousKeyStates[1])
	{
		rotation[1] = !rotation[1];
	}
	previousKeyStates[1] = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !previousKeyStates[2])
	{
		rotation[2] = !rotation[2];
	}
	previousKeyStates[2] = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !previousKeyStates[3])
	{
		rotation[3] = !rotation[3];
	}
	previousKeyStates[3] = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !previousKeyStates[4])
	{
		rotation[4] = !rotation[4];
	}
	previousKeyStates[4] = glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS;

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && !previousKeyStates[5])
	{
		rotation[5] = !rotation[5];
	}
	previousKeyStates[5] = glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS;

		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));

	for (auto tess : tesseracts)
	{	
		if (rotateTesseracts)
		{
			if (rotation[0])
				tess->rotate(0.08f, "xw");
			if (rotation[1])
				tess->rotate(0.08f, "yw");
			if (rotation[2])
				tess->rotate(0.08f, "zw");
			if (rotation[3])
				tess->rotate(0.08f, "xy");
			if (rotation[4])	
				tess->rotate(0.08f, "yz");
			if (rotation[5])	
				tess->rotate(0.08f, "xz");	
		}
		tess->draw();
	}
}