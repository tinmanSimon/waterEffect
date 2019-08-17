#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <STB_IMG/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <Simon/simon_func.h>
#include "Window.h"

class Camera
{
public:
	glm::vec3 cameraFront;
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;
	float fov;
	float fovmin;
	float fovmax;
	float fovSensitive;

	//global variables for delta time
	float currentFrame;
	float lastFrame;
	float deltaTime;
	float camProtoSpeed;

	//global variables for call back functions
	float lastX, lastY;
	float sensitivity;
	float Yaw;
	float Pitch;
	bool firstMouse = true;

	glm::mat4 view;
	glm::mat4 projection;
	float n;
	float f;

	Camera();
	~Camera();

	void update();
};

//this is the camera that is active right now
extern Camera* cam;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double x, double y);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void updateDeltaTime();

void processInput(GLFWwindow* window);

#endif // !CAMERA_H