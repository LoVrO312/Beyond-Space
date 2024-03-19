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
#include"Cube.h"
#include"Tesseract.h"

/*
compile with:
	g++ -std=c++17 Main.cpp Camera.cpp EBO.cpp VAO.cpp VBO.cpp shaderClass.cpp stb.cpp Texture.cpp Cube.cpp Tesseract.cpp src/glad.c -I./include -L./lib -lglfw3dll -o out.exe
TODO: dodaj i Mat5.cpp u compile komandu
*/


const unsigned int width = 800;
const unsigned int height = 800;
float FOV = 90.0f; // default is 45.0f

/* 
	TODO: ..... 4d .....
	TODO: spawnas teserakt koristenjem tipke E :: koraci:
		TODO: static VAO i EBO koji se inicijaliziraju na pocetku
		TODO: VBO koji se inicijalizira u konstruktoru
		TODO: (static??) draw funkcija koja koristi update vertex data za VBO 

	? Ideje:
		* Pomicanje objekata pomoću praćenja miša
		* pritiskom strelice prema gore / dole pomice se projekcija u prostoru

	VAO, VBO, EBO:
		- 1 VAO za sve objekte istog tipa (koji imaju jednako elemenata i isto raspoređene boje i pozicije itd) - npr 1 VAO za sve kocke u sceni
		- 1 VBO po objektu - npr ako imam 2 teserakta svaki ce imat zaseban VBO
		- 1 EBO za sve objekte istog tipa (kao i VAO)
*/

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

	GLFWwindow* window = glfwCreateWindow(width, height, "Tester", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


						// Cube:
	Cube cube;

	VAO VAO_cube;
	VAO_cube.Bind();
	cube.translate(-2.0f, 0.0f, -2.0f);

	VBO VBO_cube;
	cube.updateVertexData(VBO_cube); // also binds and unbinds VBO

	EBO EBO_cube(cube.wireframeIndices, sizeof(cube.wireframeIndices));

	// coordinates (location = 0) and colors (location =  1) to VAO
	VAO_cube.LinkAttrib(VBO_cube, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_cube.LinkAttrib(VBO_cube, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them (VBO is already unbound)
	VAO_cube.Unbind();
	EBO_cube.Unbind(); 


						// Tesseract: 
	Tesseract tesseract;

	VAO VAO_tes;
	VAO_tes.Bind();

	VBO VBO_tes;
	tesseract.updateVertexData(VBO_tes);

	EBO EBO_tes(tesseract.wireframeIndices, sizeof(tesseract.wireframeIndices));

	VAO_tes.LinkAttrib(VBO_tes, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO_tes.LinkAttrib(VBO_tes, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO_tes.Unbind();
	EBO_tes.Unbind();


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

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

		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(FOV, 0.1f, 100.0f, shaderProgram, "camMatrix");
		

				// Cube manipulation
		cube.rotate(0.05, 'x');
		cube.rotate(0.05, 'y');
		cube.rotate(0.05, 'z');
		cube.updateVertexData(VBO_cube);
		VAO_cube.Bind();
		glDrawElements(GL_LINES, sizeof(cube.wireframeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);


				// Tesseract manipulation
		tesseract.rotate(0.15, "xy");
		tesseract.rotate(0.15, "yz");
		tesseract.rotate(0.15, "wz");
		tesseract.updateVertexData(VBO_tes);
		VAO_tes.Bind();
		glDrawElements(GL_LINES, sizeof(tesseract.wireframeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO_cube.Delete();
	VBO_cube.Delete();
	EBO_cube.Delete();

	VAO_tes.Delete();
	VBO_tes.Delete();
	EBO_tes.Delete();
	
	shaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}