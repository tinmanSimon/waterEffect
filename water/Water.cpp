#include "Water.h"

using namespace std;
using namespace glm;

bool Water::geometry = false;

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
	initShader();

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
	//config for vertex shader
	shader->use();
	shader->setmat4(model, "model");
	shader->setmat4(view, "view");
	shader->setmat4(proj, "projection");
	shader->setFloat(t, "t");

	//config for frag shader
	shader->setVec3(0.0f, 0.467f, 0.745f, "albedo");
	shader->setFloat(0, "metallic");
	shader->setFloat(0, "roughness");
	shader->setFloat(0.5, "ao");
	shader->setVec3(0.5, 0.5, 0.5, "lightPosition");
	shader->setVec3(1.0f, 1.0f, 1.0f, "lightColor");
	shader->setVec3(cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z, "camPos");
}

void Water::initShader() {
	shader = new Shader(vertex, frag, geo);

	//init vertex shader
	string tmp;
	int waveSize = 10;
	shader->setInt(waveSize, "waveSize");
	forUp(i, waveSize) {
		vec2 direction = vec2(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f));
		float A = randFloat(0, 0.2f); //waves amplitude
		float speed = randFloat(6.0f, 9.0f); //waves speed
		float wavelength = randFloat(1.0f, 3.0f); 
		float w = speed / wavelength;
		float Q = randFloat(0, 1/(w*A));
		float phase_constant = randFloat(0, wavelength / 2.0f);
		tmp = "waves.Ds[" + to_string(i) + "]";
		shader->setVec2(direction, tmp.c_str());
		tmp = "waves.As[" + to_string(i) + "]";
		shader->setFloat(A, tmp.c_str());
		tmp = "waves.speeds[" + to_string(i) + "]"; 
		shader->setFloat(speed, tmp.c_str());
		tmp = "waves.wavelengths[" + to_string(i) + "]"; 
		shader->setFloat(wavelength, tmp.c_str());
		tmp = "waves.ws[" + to_string(i) + "]"; //ws
		shader->setFloat(w, tmp.c_str());
		tmp = "waves.Qs[" + to_string(i) + "]"; //Qs
		shader->setFloat(Q, tmp.c_str());
		tmp = "waves.phase_constants[" + to_string(i) + "]"; //Qs
		shader->setFloat(phase_constant, tmp.c_str());
	}
	
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
	if(geometry) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, triangles.size());
}
