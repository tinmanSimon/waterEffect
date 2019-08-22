#include "Water.h"

using namespace std;
using namespace glm;

void Water::addTriangles() {
	triangles.resize((l - 1) * (w - 1) * 6);
	int triangleCount = 0;
	forUp(i, vertices.size() - 1) {
		forUp(j, vertices[i].size() - 1) {
			triangles[triangleCount++] = vertices[i][j];
			triangles[triangleCount++] = vertices[i][j+1];
			triangles[triangleCount++] = vertices[i+1][j+1];

			triangles[triangleCount++] = vertices[i][j];
			triangles[triangleCount++] = vertices[i + 1][j + 1];
			triangles[triangleCount++] = vertices[i + 1][j];
		}
	}
}

void Water::addTrianglesToBuffer() {
	vao->bufferData(&triangles[0], triangles.size()*sizeof(vec3));
	vao->addAttribute(0, 3, 3*sizeof(float), 0);
}

void Water::createShader(const char* vertex, const char* frag) {
	//shader = Shader(vertex, frag);
}

void Water::waterLogic() {
	model = translate(mat4(1.0f), vec3(0, -1.0f, 0));
	view = cam->view;
	proj = cam->projection;

	//Todo change t to real time simulation
	t = glfwGetTime();
}

Water::Water(int width, int length, float s) : 
	//note width + 1 because we want start from 0 and end up with w, so w + 1 lines. Same for l.
	w{ width + 1 }, l{ length + 1 }, verticesSpace{ s }, model{ mat4(1) } 
{
	assert(width > 0, length > 0, s > 0);

	//construct vao and shader
	vao = new VAO(false);
	shader = new Shader(vertex, frag, geo);

	//construct vertices. 
	vertices.resize(l);
	forUp(i, l) { 
		float z = (float)(i - (l / 2)) * verticesSpace;
		vertices[i].resize(w, vec3(1,0,z));
		forUp(j, vertices[i].size()) {
			vertices[i][j].x = (float) (j - (w / 2)) * verticesSpace ;
		}
	}
	addTriangles();
	addTrianglesToBuffer();
}

void Water::useShader() {
	shader->use();
	shader->setmat4(model, "model");
	shader->setmat4(view, "view");
	shader->setmat4(proj, "projection");
	shader->setFloat(t, "t");

	//configue for frag shader
	shader->setVec3(0.0f, 0.467f, 0.745f, "albedo");
	shader->setFloat(0, "metallic");
	shader->setFloat(0, "roughness");
	shader->setFloat(0.5, "ao");
	shader->setVec3(0.5, 0.5, 0.5, "lightPosition");
	shader->setVec3(1.0f, 1.0f, 1.0f, "lightColor");
	shader->setVec3(cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z, "camPos");
}

Water::~Water()
{
	free(vao);
	free(shader);
}

void Water::draw() {
	waterLogic();
	vao->use();
	useShader();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, triangles.size());
}
