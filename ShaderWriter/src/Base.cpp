#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <shader.h>
#include <iostream>
#include <cstdio>

unsigned int VBO, VAO, EBO;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void renderSphere();



const unsigned int W_width = 1024;
const unsigned int W_height = 780;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

bool firstMouse = true;
float lastX = 1024.0f / 2.0f;
float lastY = 720.0f / 2.0f;


float deltaTime = 0.0f;
float lastFrame = 0.0f;




/*
 * This function handles user input via the keyboard translate the camera position based on the pressed keys,
 *  and closes the window if the ESCAPE key is pressed.
 *
 * @param window: A pointer to the GLFWwindow object representing the application window.
 */
void processInput(GLFWwindow* window)
{
	// Set the camera movement speed
	float cameraSpeed = 2.5f * deltaTime;

	// If the ESCAPE key is pressed, close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// If the W key is pressed, move the camera forward 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;

	// If the S key is pressed, move the camera backward 
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;;

	// If the A key is pressed, move the camera left 
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	// If the D key is pressed, move the camera right 
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	// If the E key is pressed, move the camera upward
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed;

	// If the Q key is pressed, move the camera downward
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= cameraUp * cameraSpeed;
}

/*
 * This function is a callback that adjusts the field of view (fov) of the camera based on
 * the scroll wheel movement. 
 *
 * @param window: A pointer to the GLFWwindow object representing the application window.
 * @param xoffset: The horizontal scroll offset
 * @param yoffset: The vertical scroll offset
 */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Adjust the field of view based on the vertical scroll wheel movement
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;

	// Clamp the field of view to a minimum of 1.0f degrees
	if (fov <= 1.0f)
		fov = 1.0f;

	// Clamp the field of view to a maximum of 45.0f degrees
	if (fov >= 45.0f)
		fov = 45.0f;
}


/*
 * This function is a callback which allows the user to control the camera's orientation
 * by moving the mouse. The pitch angle is clamped between -89.0f and 89.0f degrees
 * to prevent the camera from flipping upside down.
 *
 * @param window: A pointer to the GLFWwindow object representing the application window.
 * @param xpos: The current x-coordinate.
 * @param ypos: The current y-coordinate.
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Initialize lastX and lastY on the first call to this function
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// Calculate the mouse movement offsets since the last call to this function
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// Update the camera's yaw and pitch angles based on the mouse movement offsets
	yaw += xoffset;
	pitch += yoffset;

	// Clamp the pitch angle between -89.0f and 89.0f degrees to prevent the camera flipping upsidedown
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Update the camera's front vector based on the updated yaw and pitch angles
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}


int main(int argc, char** argv)
{

	// Create window
	glfwInit();


	GLFWwindow* window = glfwCreateWindow(W_width, W_height, "Project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Create OpenGL context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (glewInit()) {
		std::cout << "Failed to Load OpenGL" << std::endl;
		return -1;
	}

	
	glEnable(GL_DEPTH_TEST);

	// Define light positions and the colors associated to them
	glm::vec3 lightPositions[] = {
	   glm::vec3(0.0f, 0.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(150.0f, 150.0f, 150.0f),
	};
	
	// Set the shader directory to the given paths
	Shader shader("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\PBRvs.txt", "C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\PBRfs.txt");

	// Set the current shader to the active context
	shader.use();

	// Write the PBR shader
	shader.createMaterial(lightPositions,lightColors, sizeof(lightPositions));

	// Set the PBR shaders textures
	shader.setMaps("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\textures\\flooring_albedo.png",
		"C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\textures\\flooring_normal-ogl.png",
		"C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\textures\\flooring_metallic.png",
		"C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\textures\\flooring_roughness.png",
		"C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\textures\\flooring_ao.png");

	shader.makeSubmerged();
	

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set to active context and update the shader each frame
		shader.use();
		shader.updateMaterial(cameraPos, cameraFront, cameraUp);
		renderSphere();
		shader.updateSubmerged();
		

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}


unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359f;
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow)
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = static_cast<unsigned int>(indices.size());

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		unsigned int stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
