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
GLASSGROUND* glass;

vector<Sphere*> render_spheres;

vector<RenderObject*> Drawer::renderObjects;
//vector<Water*> Drawer::renderObjects;

float g_const = 9.8f;
float t; //time is changing
float delta_t;
bool runAnimation = false;
float threshold = 0.001f;
float energyConsumptionRate = 0.75f;

FrameBuffer* fbo;
extern Window* the_window;
extern vec3 clearColor;

Drawer::Drawer(){
}


Drawer::~Drawer() {
	for (auto obj : renderObjects) delete obj;
	delete fbo;
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
	player = new Sphere(vec3(0,20,0), 1, 400, 100);
	glass = new GLASSGROUND(4.0f, 40.0f);
	renderObjects.push_back(new Water(600, 600, 0.6f));
	renderObjects.push_back(new WATERGROUND(-2.0f));
	renderObjects.push_back(new WATERGROUND(-200.0f));
	/*
	forUp(i, 10) {
		render_spheres.push_back(new Sphere(vec3(i * 6 - 30, i * 10 + 10, i * 6 - 30), 1, 400, 100));
		renderObjects.push_back(render_spheres[i]);
	}
	*/
	renderObjects.push_back(player);
	renderObjects.push_back(glass);

	fbo = new FrameBuffer(the_window->width, the_window->height);
}

bool sphereIsStatic(Sphere* s, GLASSGROUND* g) {
	float groundAltitude = g->getAltitude();
	float edge = g->getEdge();
	vec3 sphereOrigin = s->getWorldPos();
	if (sphereOrigin.x < -edge || sphereOrigin.x > edge || sphereOrigin.z < -edge || sphereOrigin.z > edge) return false;

	float diff = abs(sphereOrigin.y - groundAltitude) - s->getRadius();

	if (abs(diff) <= 0.02f && length(s->velocity) < 0.1f) {
		//cout << "sphereOrigin.y = " << sphereOrigin.y << ", radius: " << s->getRadius() << ", diff = " << diff << ", velocity length = " << length(s->velocity) << endl;
		return true;
	}
	//cout << "sphereOrigin.y = " << sphereOrigin.y << ", radius: " << s->getRadius() << ", diff = " << diff << ", velocity length = " << length(s->velocity) << endl;

	return false;

}

void updateSphere(Sphere* s) {
	if (sphereIsStatic(s, glass)) return;
	s->sphere_translate(s->velocity * delta_t + 0.5f * g_const * delta_t * delta_t * vec3(0, -1.0f, 0));
	s->velocity += g_const * delta_t * vec3(0, -1.0f, 0);
}

void collisionDetect(Sphere* s, GLASSGROUND* g) {
	if (sphereIsStatic(s, g)) return;

	float groundAltitude = g->getAltitude();
	float edge = g->getEdge();
	vec3 sphereOrigin = s->getWorldPos();
	if (sphereOrigin.x < -edge || sphereOrigin.x > edge || sphereOrigin.z < -edge || sphereOrigin.z > edge) return;

	//now we only test its altitude
	float diff = abs(sphereOrigin.y - groundAltitude) - (s->getRadius() - threshold);
	//cout << "sphereOrigin.y = " << sphereOrigin.y << ", radius: " << s->getRadius() << ", diff = " << diff << endl;
	if (diff <= 0) {
		//we are hit
		//runAnimation = false;
		s->sphere_translate(vec3(0, -diff, 0));
		s->velocity = reflect(s->velocity, vec3(0, 1, 0));
		s->velocity = energyConsumptionRate * s->velocity;
		if (length(s->velocity) < 0.1f)s->velocity = vec3(0);
	}

}



void logic() {
	updateDeltaTime();

	delta_t = t;
	t = glfwGetTime();
	delta_t = t - delta_t;
	if (runAnimation) {
		updateSphere(player);
		collisionDetect(player, glass);

		/*
		forUp(i, render_spheres.size()) {
			updateSphere(render_spheres[i]);
			collisionDetect(render_spheres[i], glass);
		}
		*/
	}

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