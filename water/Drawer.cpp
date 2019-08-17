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

vector<RenderObject*> Drawer::renderObjects;
//vector<Water*> Drawer::renderObjects;

Drawer::Drawer()
{
}


Drawer::~Drawer()
{
}

float mydvertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
VAO vao;
Shader shader;

void Drawer::drawerinit() {
	//init simone func
	initRand();

	//init camera
	cam = new Camera();

	//init water
	renderObjects.push_back(new Water(300, 300, 0.5f));
}

void logic() {
	updateDeltaTime();

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