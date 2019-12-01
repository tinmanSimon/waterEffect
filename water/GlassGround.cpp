#include "GlassGround.h"
using namespace std;
using namespace glm;

static mat4 model, view, proj;
extern Sphere* player;
extern vector<Sphere*> render_spheres;
extern vec3 sunLightDir;
extern bool particleDemo;

void GLASSGROUND::addTrianglesToBuffer() {
	vao->bufferData(&triangles[0], triangles.size() * sizeof(vec3));
	vao->addAttribute(0, 3, 6 * sizeof(float), 0);
	vao->addAttribute(1, 2, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

GLASSGROUND::GLASSGROUND(float a, float e) : altitude{ a }, edge{ e } {
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

	albedoMap = new Texture("copper-rock1-alb.png");
}

GLASSGROUND::~GLASSGROUND() {
	delete vao;
	delete shader;
	delete text;
}

void GLASSGROUND::useShader(Shader* shader, Texture* text) {
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

	//vec3 player_pos = player->getWorldPos();
	//shader->setVec3(player_pos.x, player_pos.y, player_pos.z, "spherePos");
	//shader->setFloat(player->getRadius(), "sphereRadius");

	shader->setInt(render_spheres.size(), "spheres.size");
	forUp(i, render_spheres.size()) {
		Sphere* curSphere = render_spheres[i];
		string tmp = "spheres.sphere_pos[" + to_string(i) + "]";
		shader->setVec3(curSphere->getWorldPos(), tmp.c_str());
		tmp = "spheres.sphere_Radius[" + to_string(i) + "]";
		shader->setFloat(curSphere->getRadius(), tmp.c_str());
	}

	//for texture
	shader->setInt(0, "texture0");
	shader->setInt(1, "albedoMap");
	shader->setInt(10, "shadowMap"); //10 is for shadow map only
	text->use(GL_TEXTURE0);
	albedoMap->use(GL_TEXTURE1);
	//player->getAlbedoTexture()->use(GL_TEXTURE1);
}

void GLASSGROUND::draw() {
	if (particleDemo) return;
	vao->use();
	useShader(shader, text);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, traingleSize);
	glDisable(GL_BLEND);
}

void GLASSGROUND::drawShadow(Shader* s) {
	vao->use();
	s->setmat4(model, "model");
	s->use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, traingleSize);
	glDisable(GL_BLEND);
}

float GLASSGROUND::getAltitude() {
	return altitude;
}

float GLASSGROUND::getEdge() {
	return edge;
}

