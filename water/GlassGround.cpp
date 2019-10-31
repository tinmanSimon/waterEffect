#include "GlassGround.h"
using namespace std;
using namespace glm;

const float edge = 20;
static mat4 model, view, proj;

void GLASSGROUND::addTrianglesToBuffer() {
	vao->bufferData(&triangles[0], triangles.size() * sizeof(vec3));
	vao->addAttribute(0, 3, 6 * sizeof(float), 0);
	vao->addAttribute(1, 2, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

GLASSGROUND::GLASSGROUND(float a) : altitude{ a } {
	//add triangles
	triangles.push_back(vec3(-edge, a, edge));
	triangles.push_back(vec3(0, 1, 0)); //this part is for texture coordinates
	triangles.push_back(vec3(-edge, a, -edge));
	triangles.push_back(vec3(0, 0, 0));
	triangles.push_back(vec3(edge, a, -edge));
	triangles.push_back(vec3(1, 0, 0));

	triangles.push_back(vec3(-edge, a, edge));
	triangles.push_back(vec3(0, 1, 0));
	triangles.push_back(vec3(edge, a, edge));
	triangles.push_back(vec3(1, 1, 0));
	triangles.push_back(vec3(edge, a, -edge));
	triangles.push_back(vec3(1, 0, 0));

	traingleSize = triangles.size();

	vao = new VAO(false);
	shader = new Shader(vertex, frag);
	text = new Texture(text_file_name, "", "", GL_RGBA, 1);
	addTrianglesToBuffer();
	triangles.clear();
}

GLASSGROUND::~GLASSGROUND() {
	delete vao;
	delete shader;
	delete text;
}

void useShader(Shader* shader, Texture* text) {
	model = mat4(1);
	view = cam->view;
	proj = cam->projection;

	shader->use();
	shader->setmat4(model, "model");
	shader->setmat4(view, "view");
	shader->setmat4(proj, "projection");

	//for texture
	shader->setInt(0, "texture0");
	text->use(GL_TEXTURE0);
}

void GLASSGROUND::draw() {
	vao->use();
	useShader(shader, text);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, traingleSize);
}