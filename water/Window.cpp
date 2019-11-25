#include "Window.h"


using namespace std;

glm::vec3 clearColor = glm::vec3(0.529f, 0.808f, 0.922f);
Window* the_window;
extern bool runAnimation;
int gameMode = 0; //0 is third person, 1 is free roam
extern Sphere* player;
extern GLASSGROUND* glass;
extern float time_speed;
extern float max_velocity;
extern float velocity_step;
extern bool jump;
extern float velocity_dash;
extern float fogDistance;
extern bool slow_motion_enabled;
extern vector<Sphere*> render_spheres;
extern bool useFrameBuffer;

Window::Window(char* window_name, float _width, float _height)
{
	name = window_name;
	width = _width;
	height = _height;
}


Window::~Window()
{
	glfwTerminate();
}

/*
void cleanup() {
	for (int i = 0; i < VAOS.size(); ++i)
		free(VAOS[i]);
}
*/

//this is for callback.
void processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		Water::geometry = !Water::geometry;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		runAnimation = !runAnimation;
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		useFrameBuffer = !useFrameBuffer;
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		gameMode = 1 - gameMode;
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (time_speed < 1.0f) time_speed = 1.0f;
		else time_speed += 1.0f;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		if(time_speed > 1.0f) time_speed -= 1.0f;
		else if(time_speed > 0.2f) time_speed -= 0.1f;
	}

	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		slow_motion_enabled = !slow_motion_enabled;
	}

	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if(fogDistance > cam->n) fogDistance -= 1.0f;
	}

	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if (fogDistance < 5 * cam->f) fogDistance += 1.0f;
	}
	 
	if (gameMode == 0 && key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		float player_altitude = player->getWorldPos().y;
		float glass_altitude = glass->getAltitude();
		if (player_altitude > glass_altitude && (player_altitude < glass_altitude + player->getRadius() * 2 + 1)) {
			jump = true;
		}
	}

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
		if (player != NULL && gameMode == 0) {
			player->velocity += velocity_dash * normalize(cam->cameraFront);
		}
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		forUp(i, render_spheres.size()) {
			render_spheres[i]->reset();
		}
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		forUp(i, render_spheres.size()) {
			render_spheres[i]->reset(glm::vec3(i*2, 8, i*2));
		}
	}
}

//this processes directly, so it makes movement smooth
void processMovementInputs(GLFWwindow* window) {

	//original gameplay mode
	if (player != NULL && gameMode == 0) {
		float cameraSpeed = cam->camProtoSpeed * cam->deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			glm::vec3 horizontal_camfront = glm::normalize(glm::vec3(cam->cameraFront.x, 0, cam->cameraFront.z));
			float player_proj = glm::dot(player->velocity, horizontal_camfront);
			if (player_proj <= max_velocity) player->velocity += horizontal_camfront * velocity_step;
		}
			
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			glm::vec3 horizontal_camfront = -glm::normalize(glm::vec3(cam->cameraFront.x, 0, cam->cameraFront.z));
			float player_proj = glm::dot(player->velocity, horizontal_camfront);
			if (player_proj <= max_velocity) player->velocity += horizontal_camfront * velocity_step;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			glm::vec3 side = -normalize(cross(cam->cameraFront, cam->cameraUp));
			glm::vec3 horizontal_camside = glm::normalize(glm::vec3(side.x, 0, side.z));
			float player_proj = glm::dot(player->velocity, horizontal_camside);
			if (player_proj <= max_velocity) player->velocity += horizontal_camside * velocity_step;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			glm::vec3 side = normalize(cross(cam->cameraFront, cam->cameraUp));
			glm::vec3 horizontal_camside = glm::normalize(glm::vec3(side.x, 0, side.z));
			float player_proj = glm::dot(player->velocity, horizontal_camside);
			if (player_proj <= max_velocity) player->velocity += horizontal_camside * velocity_step;
		}
	}

	//free roaming mode
	else if (cam != NULL && gameMode == 1) {
		float cameraSpeed = cam->camProtoSpeed * cam->deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam->cameraPos += cameraSpeed * cam->cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam->cameraPos -= cameraSpeed * cam->cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam->cameraPos -= normalize(cross(cam->cameraFront, cam->cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam->cameraPos += normalize(cross(cam->cameraFront, cam->cameraUp)) * cameraSpeed;
	}

	
	
	else cout << "cam == NULL!" << endl;
}

int Window::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(width, height, name, NULL, NULL);



	if (window == NULL)
	{
		cout << "ERROR: GLFW Window creation failed!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: GLAD Initialization failed!" << std::endl;
		return -1;
	}

	//bunch of call back functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, processInput);


	glEnable(GL_DEPTH_TEST); //z buffer
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //remove cursor

	//MSAA
	glEnable(GL_MULTISAMPLE);


	glViewport(0, 0, width, height);

	//init drawer
	Drawer::drawerinit();

	

	return 0;
}


void Window::loop() {

	while (!glfwWindowShouldClose(window)) {
		processMovementInputs(window);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		logic();
		Drawer::draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cleanup();
}