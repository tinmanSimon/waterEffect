#include "Drawer.h"

using namespace std;
using namespace glm;

bool pause = false;

unsigned int framebuffer; //for fbo test only
unsigned int intermediateFBO; //for fbo test only
unsigned int texColorBuffer;
unsigned int rbo;

Texture* tForFbo;
Texture* tForIFbo;
VAO* msaaVAO;
Shader* msaaShader;
Sphere* player;

vector<RenderObject*> Drawer::renderObjects;
//vector<Water*> Drawer::renderObjects;

float g_const = 9.8f;
float t; //time is changing
float delta_t;

Drawer::Drawer()
{
}


Drawer::~Drawer()
{
}

void Drawer::drawerinit() {
	//init simone func
	initRand();

	//init camera
	cam = new Camera();

	//init time
	t = glfwGetTime();

	//init water
	Water::geometry = false;
	player = new Sphere(vec3(0,100,-50), 1, 400, 100);
	renderObjects.push_back(new Water(600, 600, 0.6f));
	renderObjects.push_back(new WATERGROUND(-2.0f));
	renderObjects.push_back(new WATERGROUND(-200.0f));
	renderObjects.push_back(player);
	renderObjects.push_back(new GLASSGROUND(4.0f));
}

void updateSphere(Sphere* s) {
	s->sphere_translate(s->velocity * delta_t + 0.5f * g_const * delta_t * delta_t * vec3(0, -1.0f, 0));
	s->velocity += g_const * delta_t * vec3(0, -1.0f, 0);
}

void logic() {
	updateDeltaTime();

	delta_t = t;
	t = glfwGetTime();
	delta_t = t - delta_t;
	//updateSphere(player);

	//update camera
	cam->update();
	
}

void Drawer::draw() {
	forUp(i, renderObjects.size()) renderObjects[i]->draw();
}

void Drawer::freeRenderObjects() {
	forUp(i, renderObjects.size()) free(renderObjects[i]);
}

void cleanup() {
	for (int i = 0; i < VAOS.size(); ++i) {
		free(VAOS[i]);
	}
	for (int i = 0; i < shaders.size(); ++i) {
		free(shaders[i]);
	}
	for (int i = 0; i < globalTextures.size(); ++i) {
		free(globalTextures[i]);
	}
	for (int i = 0; i < spheres.size(); ++i) {
		free(spheres[i]);
	}

	Drawer::freeRenderObjects();

	free(cam);
}