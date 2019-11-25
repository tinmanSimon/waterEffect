#include "WaterGround.h"
using namespace std;
using namespace glm;

const float edge = 10000;
static mat4 model, view, proj;
extern vec3 sunLightDir;

void WATERGROUND::addTrianglesToBuffer() {
	vao->bufferData(&triangles[0], triangles.size() * sizeof(vec3));
	vao->addAttribute(0, 3, 3 * sizeof(float), 0);
	//vao->addAttribute(1, 2, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

WATERGROUND::WATERGROUND(float a) : altitude{ a }, traingleSize{ 0 } {
	//add triangles
	triangles.push_back(vec3(-edge, a, edge));
	triangles.push_back(vec3(-edge, a, -edge));
	triangles.push_back(vec3(edge, a, -edge));

	triangles.push_back(vec3(-edge, a, edge));
	triangles.push_back(vec3(edge, a, edge));
	triangles.push_back(vec3(edge, a, -edge));

	traingleSize = triangles.size();

	vao = new VAO(false);
	shader = new Shader(vertex, frag);
	addTrianglesToBuffer();
	triangles.clear();
}

WATERGROUND::~WATERGROUND() {
	delete vao;
	delete shader;
}

void useShader(Shader* shader) {
	model = mat4(1);
	view = cam->view;
	proj = cam->projection;

	shader->use();
	shader->setmat4(model, "model");
	shader->setmat4(view, "view");
	shader->setmat4(proj, "projection");

	shader->setVec3(sunLightDir.x, sunLightDir.y, sunLightDir.z, "lightPosition");
	shader->setVec3(1, 1, 0, "lightColor");
	shader->setVec3(cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z, "camPos");

	shader->setInt(10, "shadowMap"); //10 is for shadow map only
}

void WATERGROUND::draw() {
	vao->use();
	useShader(shader);
	glDrawArrays(GL_TRIANGLES, 0, traingleSize);
}

void WATERGROUND::drawShadow(Shader* s){
	vao->use();
	s->setmat4(model, "model");
	s->use();
	glDrawArrays(GL_TRIANGLES, 0, traingleSize);
}