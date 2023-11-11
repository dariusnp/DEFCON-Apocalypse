// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

//variables
GLFWwindow* window;
const int width = 1024, height = 768;

// lighting
glm::vec3 lightPos(-3.0f, 0.0f, 3.0f);
glm::vec3 lightColor(1.0f);

//Time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Add the camera parameters here and create a camera object

void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "3D demo", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create and compile our GLSL program from the shaders
	// More about this in the following labs
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	GLuint programID2 = LoadShaders("LightVertexShader.vertexshader", "LightFragmentShader.fragmentshader");


	float vertices[] = {
		0.0f,  0.0f, 0.05f,  // top right
		0.05f, 0.0f, 0.05f,  // bottom right
		0.0f, 0.05f, 0.05f,  // bottom left
		0.05f,  0.05f, 0.05f,   // top left 

		0.0f, 0.0f, 0.0f,
		0.05f, 0.0f, 0.0f,
		0.0f, 0.05f, 0.0f,
		0.05f, 0.05f, 0.0f
	};


	unsigned int indices[] = {  
		0, 1, 2,  
		1, 3, 2, 
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4
	};

	glm::vec3 positions[] = {
		glm::vec3(0.0f,  0.0f,  0.2f),
		glm::vec3(0.2f,  0.5f, 0.1f),
		glm::vec3(-0.15f, -0.22f, 0),
		glm::vec3(-0.38f, -0.2f, -0.7),
		glm::vec3(0.24f, -0.4f, 0.1),
		glm::vec3(-0.17f,  0.3f, 0.7),
		glm::vec3(0.23f, -0.2f, 0.1),
		glm::vec3(0.15f,  0.2f, 0),
		glm::vec3(0.7f,  0.7f, 0.9),
		glm::vec3(-0.13f,  0.9f, 0)
	};

	//vertices and indices for a light source (pyramid)
	float verticesPyramid[] = {
		0.05f,  0.0f, 0.05f,  
		-0.05f, 0.0f, 0.05f,  
		-0.05f, 0.0f, -0.05f,  
		0.05f,  0.0f, -0.05f,   //base

		0.0f, 0.1f, 0.0f //top
	};

	unsigned int indicesPyramid[] = { 
		0, 2, 1,
		0, 3, 2,
		0, 3, 4,
		0, 4, 1,
		1, 4, 2,
		3, 4, 2
	};

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store 
	// the information for a complete rendered object.
	GLuint vbo1, vao1, ibo1,
		vbo2, vao2, ibo2;

	//Bindings for cubes
	glGenVertexArrays(1, &vao1);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &ibo1);

	glBindVertexArray(vao1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//Unbinding VAO
	glBindVertexArray(0);

	//Bindings for pyramid
	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);
	glGenBuffers(1, &ibo2);

	glBindVertexArray(vao2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPyramid), verticesPyramid, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPyramid), indicesPyramid, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//Unbinding VAO
	glBindVertexArray(0);

	glfwSetFramebufferSizeCallback(window, window_callback);

	//Change accordingly for camera
	glm::mat4 view;
	view = glm::lookAt(glm::vec3(0.0f, 30.0f, 100.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);

	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		//calculate delta time 
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame; 

		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		for (int i = 0; i < 10; i++)
		{

			glm::mat4 model;
			model = glm::translate(model, positions[i]);
			model = glm::rotate(model, (float)glfwGetTime() * 100, glm::vec3(0.0f, 1.0f, 0.0f));

			//calculate MVP matrix
			glm::mat4 matrix = projection * view * model;

			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

			glm::vec4 color = glm::vec4(0.5f + i / 10.0f, 1 - i / 10.0f, 0.5f, 1.0);
			unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
			glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

			//!!! bind the appropriate VAO before drawing
			glBindVertexArray(vao1);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		//Light object changes
		//let's play with the other vao

		glUseProgram(programID2);
		lightPos.x = sin(glfwGetTime());
		lightPos.y = sin(glfwGetTime() / 8.0f) * 1.0f + 0.5f;


		glm::mat4 matrix2 = glm::mat4(1.0f);

		matrix2 = glm::translate(matrix2, lightPos);

		matrix2 = glm::scale(matrix2, glm::vec3(2.0f));

		glm::mat4 mpv = projection * view * matrix2;

		unsigned int transformLoc = glGetUniformLocation(programID2, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mpv));

		unsigned int transformLoc2 = glGetUniformLocation(programID2, "color");
		glUniform4fv(transformLoc2, 1, glm::value_ptr(lightColor));

		glBindVertexArray(0);
		glBindVertexArray(vao2);
		//What is the function call for drawing the pyramid?

	}

	// Cleanup VBO
	glDeleteBuffers(1, &vbo1);
	glDeleteBuffers(1, &vbo2);
	glDeleteBuffers(1, &ibo1);
	glDeleteBuffers(1, &ibo2);
	glDeleteVertexArrays(1, &vao1);
	glDeleteVertexArrays(1, &vao2);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}