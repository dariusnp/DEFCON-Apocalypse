#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"
#include "UI/UI.hpp"
#include <unordered_map>
#include <vector>

using namespace sas;

struct hitBoxes
{
	float xMin, xMax, zMin, zMax;
};

float yaw = -90.f, pitch = 0.f, M_PI = 3.1415f, Xdegrees = 0.f, Ydegrees = 0.f, speed = 250, stamina = 100.f, lastPressTime = 0.f, minPressCooldown = 0.75f;
double lastX = 0, lastY = 0;
//int health = 100;
const float heightMax = 48.f, heightMin = 0.f, jumpHeight = 10.f;
const float npcX = 40.f, npcZ = -23.f;
bool isJumping = true, isExhausted = false, canMove = true;
bool canPress = true;


void processKeyboardInput (UI& ui, const std::unordered_map<sas::GameState, sas::GameState>& states);
void mouse_callback (GLFWwindow* window, double xpos, double ypos) noexcept;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Window window("Jocuuuul", 1200, 1200);
Camera camera;

sas::GameState currGame = sas::GameState::MainMenu;

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-180.0f, 1984.0f, -200.0f);

hitBoxes boxes[9] = { {-31, 165, 163, 264}, {36, 116, 45, 89}, {369, 412, 83, 153}, {517, 520, 49, 118}, {480, 522, 48, 117}, {480, 520, 115, 118}, {-121, -51, -237, -163}, {-50, 50, -247, -150}, {50, 121, -236, -163} };

const std::unordered_map<sas::GameState, sas::GameState> states{
		{sas::GameState::MainMenu, sas::GameState::Exit},
		{sas::GameState::Options, sas::GameState::MainMenu},
		{sas::GameState::MainGame, sas::GameState::PauseMenu},
		{sas::GameState::PauseMenu, sas::GameState::MainGame},
		{sas::GameState::GameEnding, sas::GameState::PauseMenu},
		{sas::GameState::PauseOptions, sas::GameState::PauseMenu}
};

bool isHit(float sX, float sY, float sZ, float enemAngle, float objX, float objZ)
{
	//float distance = std::sqrt((sX - objX) * (sX - objX) + (sZ - objZ) * (sZ - objZ));
	float enemAngleMod = 90 - enemAngle;
	while (enemAngleMod < 0)
		enemAngleMod += 360;
	return std::abs(enemAngleMod - sY) <= 183.5f && std::abs(enemAngleMod - sY) >= 140.5f;
}

int main()
{

	bool renderCutie1 = true, renderCutie2 = true, renderCutie3 = true, itemTaken = false; 
	bool startTask1 = false, startTask2 = false, startTask3 = false, startTask4 = false;

	float xScaun = 650.f, zScaun = -216.f;
	glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//building and compiling shader program
	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");

	//Textures
	GLuint tex = loadBMP("Resources/Textures/wood.bmp");
	GLuint tex2 = loadBMP("Resources/Textures/rock.bmp");
	GLuint tex3 = loadBMP("Resources/Textures/orange.bmp");
	GLuint tex4 = loadBMP("Resources/Textures/ground.bmp");
	GLuint btex = loadBMP("Resources/Textures/brack.bmp");
	GLuint tex5 = loadBMP("Resources/Textures/tank.bmp");
	GLuint tex6 = loadBMP("Resources/Textures/arma.bmp");
	GLuint tex7 = loadBMP("Resources/Textures/Oras.bmp");
	GLuint tex8 = loadBMP("Resources/Textures/grass.bmp");
	GLuint tex9 = loadBMP("Resources/Textures/armor.bmp");

	glEnable(GL_DEPTH_TEST);

	//Test custom mesh loading
	std::vector<Vertex> vert;
	vert.push_back(Vertex());
	vert[0].pos = glm::vec3(10.5f, 10.5f, 0.0f);
	vert[0].textureCoords = glm::vec2(1.0f, 1.0f);

	vert.push_back(Vertex());
	vert[1].pos = glm::vec3(10.5f, -10.5f, 0.0f);
	vert[1].textureCoords = glm::vec2(1.0f, 0.0f);

	vert.push_back(Vertex());
	vert[2].pos = glm::vec3(-10.5f, -10.5f, 0.0f);
	vert[2].textureCoords = glm::vec2(0.0f, 0.0f);

	vert.push_back(Vertex());
	vert[3].pos = glm::vec3(-10.5f, 10.5f, 0.0f);
	vert[3].textureCoords = glm::vec2(0.0f, 1.0f);

	vert[0].normals = glm::normalize(glm::cross(vert[1].pos - vert[0].pos, vert[3].pos - vert[0].pos));
	vert[1].normals = glm::normalize(glm::cross(vert[2].pos - vert[1].pos, vert[0].pos - vert[1].pos));
	vert[2].normals = glm::normalize(glm::cross(vert[3].pos - vert[2].pos, vert[1].pos - vert[2].pos));
	vert[3].normals = glm::normalize(glm::cross(vert[0].pos - vert[3].pos, vert[2].pos - vert[3].pos));

	std::vector<int> ind = { 0, 1, 3,   
		1, 2, 3 };

	std::vector<Texture> textures;
	textures.push_back(Texture());
	textures[0].id = tex;
	textures[0].type = "texture_diffuse";

	std::vector<Texture> textures2;
	textures2.push_back(Texture());
	textures2[0].id = tex2;
	textures2[0].type = "texture_diffuse";

	std::vector<Texture> textures3;
	textures3.push_back(Texture());
	textures3[0].id = tex3;
	textures3[0].type = "texture_diffuse";

	std::vector<Texture> textures4;
	textures4.push_back(Texture());
	textures4[0].id = tex4;
	textures4[0].type = "texture_diffuse";

	std::vector<Texture> textures5;
	textures5.push_back(Texture());
	textures5[0].id = tex5;
	textures5[0].type = "texture_diffuse";

	std::vector<Texture> textures6;
	textures6.push_back(Texture());
	textures6[0].id = tex6;
	textures6[0].type = "texture_diffuse";

	std::vector<Texture> buildingTexture1;
	buildingTexture1.push_back(Texture());
	buildingTexture1[0].id = btex;
	buildingTexture1[0].type = "texture_diffuse";

	std::vector<Texture> mapa;
	mapa.push_back(Texture());
	mapa[0].id = tex7;
	mapa[0].type = "texture_diffuse";

	std::vector<Texture> plan2;
	plan2.push_back(Texture());
	plan2[0].id = tex8;
	plan2[0].type = "texture_diffuse";	

	std::vector<Texture> armor;
	armor.push_back(Texture());
	armor[0].id = tex9;
	armor[0].type = "texture_diffuse";



	Mesh mesh(vert, ind, textures3);

	// Create Obj files - easier :)
	// we can add here our textures :)
	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh box = loader.loadObj("Resources/Models/cube.obj", textures);
	Mesh plane = loader.loadObj("Resources/Models/plane.obj", textures4);
	Mesh man = loader.loadObj("Resources/Models/FinalBaseMesh.obj", armor);
	Mesh b1 = loader.loadObj("Resources/Models/building.obj", buildingTexture1);
	Mesh b2 = loader.loadObj("Resources/Models/Calisma2.obj", buildingTexture1);
	Mesh r1 = loader.loadObj("Resources/Models/ruin.obj", buildingTexture1);
	Mesh map = loader.loadObj("Resources/Models/map.obj", mapa);
	Mesh tank = loader.loadObj("Resources/Models/t-90a.obj", textures5);
	Mesh trailer = loader.loadObj("Resources/Models/old_trailer_obj.obj", buildingTexture1);
	Mesh carSeats = loader.loadObj("Resources/Models/Rusted_Car_Seats.obj", textures2);
	Mesh gun = loader.loadObj("Resources/Models/m4a1_s.obj", textures6);
	Mesh plane2 = loader.loadObj("Resources/Models/plane2.obj", plan2);

	UI ui(window.getWindow(), currGame, stamina);	

	//check if we close the window or press the escape button
	while (currGame != GameState::Exit && !window.isPressed(GLFW_KEY_ESCAPE) && glfwWindowShouldClose(window.getWindow()) == 0)
	{
		float playAnimation = 0;
		ui.renderUI();
		window.clear();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyboardInput(ui, states);

		if (currentFrame - lastPressTime >= minPressCooldown)
		{
			canPress = true;
			lastPressTime = currentFrame;
		}

		//test mouse input
		if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			playAnimation = 1;
		}

		if (window.isMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			std::cout << camera.getCameraPosition().x << ' ' << camera.getCameraPosition().z << '\n';
		}
		
		for (int i = 0; i < 9; i++)
		{		
			// HITBOXES
			if (camera.getCameraPosition().x + 1 > boxes[i].xMin && camera.getCameraPosition().z > boxes[i].zMin)
				camera.setCameraPosition(glm::vec3(camera.getCameraPosition().x - 1, camera.getCameraHeight(), camera.getCameraPosition().z - 1));

			if (camera.getCameraPosition().x < boxes[i].xMin && camera.getCameraPosition().x > boxes[i].xMax && camera.getCameraPosition().z < boxes[i].zMin && camera.getCameraPosition().z > boxes[i].zMax)
			{
				camera.setCameraPosition(glm::vec3(camera.getCameraPosition().x + 1, camera.getCameraHeight(), camera.getCameraPosition().z + 1));
			}
		}

		if (camera.getCameraPosition().x >= 413 && camera.getCameraPosition().x <= 422 && camera.getCameraPosition().z >= 120 && camera.getCameraPosition().z <= 135 && window.isPressed(GLFW_KEY_E))
		{
				camera.setCameraPosition(glm::vec3(650.f, camera.getCameraHeight(), -216.f));
		}

		if (camera.getCameraPosition().x >= 650 && camera.getCameraPosition().x <= 667 && camera.getCameraPosition().z >= -213 && camera.getCameraPosition().z <= -187 && window.isPressed(GLFW_KEY_E))
		{
			camera.setCameraPosition(glm::vec3(447.f, camera.getCameraHeight(), 129.f));
		}

		if (camera.getCameraPosition().x >= 515 && camera.getCameraPosition().x <= 525 && camera.getCameraPosition().z >= 85 && camera.getCameraPosition().z <= 100)
		{
			if (window.isPressed(GLFW_KEY_E))
			{
				camera.setCameraPosition(glm::vec3(camera.getCameraPosition().x - 5, camera.getCameraHeight(), camera.getCameraPosition().z));
			}
		}
		
		
		if ( std::abs(camera.getCameraPosition().x - npcX) < 20.f && std::abs(camera.getCameraPosition().z - npcZ) < 20.f)
		{
			// de schimbat cu un prompt pe ecran
			std::cout << "Apasa E ca sa interactionezi \n";

			if (window.isPressed(GLFW_KEY_E))
			{
				startTask1 = true;
			}

		}

		if (window.isPressed(GLFW_KEY_LEFT_SHIFT) && stamina > 0.5 && !isExhausted)
		{
			speed = 100;
			stamina -= 0.2f;
			if (stamina <= 1.f)
				isExhausted = true;
		}
		else
		{
			if (canMove)
				speed = 50;
		}

		if (stamina >= 0 && stamina < 100.5 && !window.isPressed(GLFW_KEY_LEFT_SHIFT))
		{
			if (!isExhausted)
				stamina += 0.5f;
			else
			if (isExhausted)
			{
				if (stamina > 99.9)
					isExhausted = false;
				stamina += 0.1f;
			}
		}
		

		if (isJumping && camera.getCameraPosition().y < heightMax)
		{
			camera.setCameraY(camera.getCameraPosition().y + jumpHeight * 15 * deltaTime);
		}
		else if (camera.getCameraPosition().y > camera.getCameraHeight())
		{
			isJumping = false;
			camera.setCameraY(camera.getCameraPosition().y - jumpHeight * 20 * deltaTime);
		}
		else
		{
			camera.setJumping(false);
		}

		if (std::abs(camera.getCameraPosition().x - xScaun) < 15 && std::abs(camera.getCameraPosition().z - zScaun) < 15)
		{
			//std::cout << itemTaken << '\n';
			if(window.isPressed(GLFW_KEY_F))
				itemTaken = true;
		}
		


		sunShader.use();

		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		GLuint MatrixID = glGetUniformLocation(sunShader.getId(), "MVP");

		//Test for one Obj loading = light source

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, lightPos);
		//ModelMatrix = glm::scale(3.f, 3.f, 3.f);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		sun.draw(sunShader);
		

		//// End code for the light ////

		shader.use();

		///// Test Obj files for box ////

		GLuint MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
		GLuint ModelMatrixID = glGetUniformLocation(shader.getId(), "model");

		// TASK 1 :::::: DISTRUGE CUTIILE



		// COD CUTIE 1
		if (startTask1)
		{
			float cutieX = -400.f, cutieZ = 30.f;
			float distance = std::pow(camera.getCameraPosition().x - cutieX, 2) + std::pow(camera.getCameraPosition().z - cutieZ, 2);
			float xDistance = (camera.getCameraPosition().x - cutieX);
			float newEnemyAngle = std::asin(xDistance / std::sqrt(distance)) / M_PI * 2 * 90;



			ModelMatrix = glm::mat4(1.0);

			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(cutieX, 0.0f, cutieZ));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			ModelMatrix = glm::rotate(ModelMatrix, 37.3f, glm::vec3(0.f, 1.f, 0.f));
			if (isHit(camera.getCameraPosition().x, yaw, camera.getCameraPosition().z, newEnemyAngle, cutieX, cutieZ) && (playAnimation > 0))
			{
				renderCutie1 = false;
			}
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

			if (renderCutie1)
			{
				box.draw(shader);
				std::cout << "Destroy the first box!";
			}

			// COD CUTIE 2
			if (!renderCutie1)
			{
				float cutieX2 = 309.f, cutieZ2 = 107.f;
				float distance2 = std::pow(camera.getCameraPosition().x - cutieX2, 2) + std::pow(camera.getCameraPosition().z - cutieZ2, 2);
				float xDistance2 = (camera.getCameraPosition().x - cutieX2);
				float newEnemyAngle2 = std::asin(xDistance2 / std::sqrt(distance2)) / M_PI * 2 * 90;



				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(cutieX2, 0.0f, cutieZ2));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				ModelMatrix = glm::rotate(ModelMatrix, 23.5f, glm::vec3(0.f, 1.f, 0.f));
				if (isHit(camera.getCameraPosition().x, yaw, camera.getCameraPosition().z, newEnemyAngle2, cutieX2, cutieZ2) && (playAnimation > 0))
				{
					renderCutie2 = false;
				}
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
				glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
				glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

				if (renderCutie2)
				{
					box.draw(shader);
					std::cout << "Destroy the second box!";
				}
			}

			// COD CUTIE 3
			if (!renderCutie2)
			{
				float cutieX2 = 260.f, cutieZ2 = -110.f;
				float distance2 = std::pow(camera.getCameraPosition().x - cutieX2, 2) + std::pow(camera.getCameraPosition().z - cutieZ2, 2);
				float xDistance2 = (camera.getCameraPosition().x - cutieX2);
				float newEnemyAngle2 = std::asin(xDistance2 / std::sqrt(distance2)) / M_PI * 2 * 90;



				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(cutieX2, 0.0f, cutieZ2));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				ModelMatrix = glm::rotate(ModelMatrix, 5.9f, glm::vec3(0.f, 1.f, 0.f));
				if (isHit(camera.getCameraPosition().x, yaw, camera.getCameraPosition().z, newEnemyAngle2, cutieX2, cutieZ2) && (playAnimation > 0))
				{
					renderCutie3 = false;
				}
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
				glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
				glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

				if (renderCutie3)
				{
					box.draw(shader);
					std::cout << "Destroy the third box!";
				}
			}

			if (!renderCutie3)
			{
				std::cout << "Talk to the captain";
				startTask1 = false;
			}
		}

		///// Test plane Obj file //////

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(8.f, 0.f, 8.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		plane.draw(shader);	

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.2f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f, 0.f, 1.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		plane2.draw(shader);
		// tank

		ModelMatrix = glm::mat4(1.0);
		//ModelMatrix = glm::rotate(45.f, glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 5.0f, 45.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.f, 2.f, 2.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		tank.draw(shader);

		// omulet

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(npcX, 0.0f, npcZ));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f, 2.5f, 1.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		man.draw(shader);

		// building

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(100.f, 40.0f, 200.f));
		//ModelMatrix = glm::rotate(90.f, glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(30.f, 40.f, 30.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		b1.draw(shader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::rotate(-45.f, glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(150.f, 40.0f, -330.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(30.f, 40.f, 30.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		b1.draw(shader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.0f, -200.f));
		//ModelMatrix = glm::rotate(90.f, glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(8.f, 10.f, 6.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		b2.draw(shader);


		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-200.f, 0.0f, 0.f));
		//ModelMatrix = glm::rotate(90.f, glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(8.f, 10.f, 6.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		r1.draw(shader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.0f, 0.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(500.f, 60.f, 500.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		map.draw(shader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(390.f, 20.0f, 115.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.f, 2.f, 2.f));
		ModelMatrix = glm::rotate(ModelMatrix, 90.f, glm::vec3(0.f, 1.f, 0.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		trailer.draw(shader);

		//RULOTA INTERIOR
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(650.f, 20.0f, -216.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.f, 2.f, 2.f));
		ModelMatrix = glm::rotate(ModelMatrix, 90.f, glm::vec3(0.f, 1.f, 0.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		trailer.draw(shader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(670.f, 5.0f, -220.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(3.f, 3.f, 3.f));
		ModelMatrix = glm::rotate(ModelMatrix, 90.f, glm::vec3(0.f, 1.f, 0.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		if(!itemTaken)
		carSeats.draw(shader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, camera.getCameraPosition().x, camera.getCameraPosition().y - 2, camera.getCameraPosition().z);
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, -0.5f));
		ModelMatrix = glm::rotate(ModelMatrix, Xdegrees, glm::vec3(0.f, 1.f, 0.f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		gun.draw(shader);

		window.update();
	}
}

void processKeyboardInput(UI& ui,  const std::unordered_map<sas::GameState, sas::GameState>& states)
{
	if (window.isPressed(GLFW_KEY_ESCAPE) && canPress)
	{
		canPress = false;
		glfwGetCursorPos(window.getWindow(), &lastX, &lastY);
		currGame = states.at(currGame);
	}

	float cameraSpeed = speed * deltaTime;

	//translation
	if (window.isPressed(GLFW_KEY_W))
		camera.keyboardMoveFront(cameraSpeed);
	if (window.isPressed(GLFW_KEY_S))
		camera.keyboardMoveBack(cameraSpeed);
	if (window.isPressed(GLFW_KEY_A))
		camera.keyboardMoveLeft(cameraSpeed);
	if (window.isPressed(GLFW_KEY_D))
		camera.keyboardMoveRight(cameraSpeed);
	/*if (window.isPressed(GLFW_KEY_LEFT_SHIFT) && camera.getCameraPosition().y > 0)
		camera.keyboardMoveDown(cameraSpeed);*/
	if (window.isPressed(GLFW_KEY_SPACE))
	{
		isJumping = true;
		camera.setJumping(true);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) noexcept
{

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.15f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.f)
		pitch = 89.f;
	if (pitch < -89.f)
		pitch = -89.f;

	if (yaw > 360)
		yaw = yaw - 360;
	if (yaw < 0)
		yaw = yaw + 360;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.setCameraViewDirection(glm::normalize(front));
	Xdegrees = 90 + 360 * glm::radians(yaw) / M_PI / 2;
	Ydegrees = -360 * glm::radians(pitch) / M_PI / 2;
}
