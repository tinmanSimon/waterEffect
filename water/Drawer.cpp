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

extern vec3 clearColor;
extern Window* window;
float t; //time is changing

vector<RenderObject*> Drawer::renderObjects;
//vector<Water*> Drawer::renderObjects;

FrameBuffer* fbo;

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

	//init frameBuffer
	fbo = new FrameBuffer(window->width, window->height);

	t = glfwGetTime(); //time is changing

	//init render objects
	Water::geometry = false;
	renderObjects.push_back(new Water(600, 600, 0.3f));
	renderObjects.push_back(new WATERGROUND(-2.0f));
	renderObjects.push_back(new WATERGROUND(-200.0f));
	renderObjects.push_back(new GLASSGROUND(4.0f));
}

void logic() {
	updateDeltaTime();

	t = glfwGetTime();

	//update camera
	cam->update();
}

void Drawer::draw() {
	fbo->useFrameBuffer(clearColor);
	forUp(i, renderObjects.size()) renderObjects[i]->draw();
	fbo->changeBackToDefaultBufferAndDraw(clearColor);
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