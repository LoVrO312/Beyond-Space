#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<libraries/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

/*
compile with:
g++ -std=c++17 Main.cpp Camera.cpp EBO.cpp VAO.cpp VBO.cpp shaderClass.cpp stb.cpp Texture.cpp src/glad.c -I./include -L./lib -lglfw3dll -o out.exe
*/


const unsigned int width = 800;
const unsigned int height = 800;
float FOV = 90.0f; // default is 45.0f

// CUBE
GLfloat vertices[] =
{ //     COORDINATES     /       COLORS 
	 1.0f,  1.0f,  1.0f,	1.0f,  0.0f,  0.0f,
	 1.0f,  1.0f, -1.0f,	0.0f,  1.0f,  0.0f,
	 1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, -1.0f,	1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,	1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, -1.0f,	1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,	1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 1.0f
};

// CUBE wireframe indices
GLuint indices[] =
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


int main()	
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "Tester", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 4.0f));

	// for displaying FPS
	double prevTime = 0.0;
	double crntTime = 0.0;
	double TimeDiff;
	// keeps track of the amount of frames in TimeDiff
	unsigned int counter = 0;

	// Enable VSync (if disabling uncomment if statement in the fps counter and change the speed of the camera)
	glfwSwapInterval(1);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// FPS counter
		crntTime = glfwGetTime();
		TimeDiff = crntTime - prevTime;
		counter++;
		if (TimeDiff >= 1.0 / 15.0)
		{
			std::string FPS = std::to_string((1.0 / TimeDiff) * counter);
			std::string ms = std::to_string((TimeDiff / counter) * 1000);
			std::string newTitle = "Hyperspace  |  FPS: " + FPS + "  |  ms: " + ms;
			glfwSetWindowTitle(window, newTitle.c_str());

			prevTime = crntTime;
			counter = 0;

			// Use this if you have disabled VSync
			// camera.Inputs(window); 
		}

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(FOV, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_LINES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}