#include "WaterGround.h"
using namespace std;
using namespace glm;

const float edge = 10000;
mat4 model, view, proj;

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

	shader->setVec3(0, 0.5, 0.8, "lightPosition");
	shader->setVec3(1, 1, 0, "lightColor");
	shader->setVec3(cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z, "camPos");
}

void WATERGROUND::draw() {
	vao->use();
	useShader(shader);
	glDrawArrays(GL_TRIANGLES, 0, traingleSize);
}