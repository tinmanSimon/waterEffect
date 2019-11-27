#include "Particle.h"
using namespace glm;
using namespace std;

/*
float particleVertices[] = {
	-0.05f,  0.0f, 0,
	 0.05f, 0.f, 0,
	0.f, 0.05f, 0
};
*/

const long particleCount = 2000000;
const float particle_size = 0.0025;
extern bool runAnimation;
static float t;
extern float delta_t;
extern float t_diff;

extern int waveSize;
extern vec2 water_directions[100];
extern float water_As[100];
extern float water_Qs[100];
extern float water_speeds[100];
extern float water_wavelengths[100];
extern float water_ws[100];
extern float water_phase_constants[100];

extern mat4 water_model;

float particleVertices[] = {
	-1.155, 0, -1,
	0,1.7,-0.333,
	0,0,1,

	0,0,1,
	0,1.7,-0.333,
	1.155,0,-1,

	1.155,0,-1,
	-1.155,0,-1,
	0,1.7,-0.333,

	-1.155,0,-1,
	0,0,1,
	1.155,0,-1
};

void Particle::initShader() {
	string tmp;
	shader = new Shader(vertex, frag);
	shader->setInt(waveSize, "waves.waveSize");
	//vec2 dirct = vec2(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f));
	forUp(i, waveSize) {
		vec2 direction = water_directions[i];
		//vec2 direction = vec2(dirct.x + randFloat(-0.4f, 0.4f), dirct.y + randFloat(-0.4f, 0.4f));
		float A = water_As[i]; //waves amplitude
		float speed = water_speeds[i]; //waves speed
		float wavelength = water_wavelengths[i];
		float w = water_ws[i];
		float Q = water_Qs[i];
		float phase_constant = water_phase_constants[i];
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

Particle::Particle() {
	//preprocessing
	forUp(i, 36) particleVertices[i] *= particle_size;

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	initShader();
	model = mat4(1);
	model = translate(model, vec3(0, 8, 0));
	t = 0;

	albedoMap = new Texture("copper-rock1-alb.png");
}


Particle::~Particle() {
	delete shader;
	delete albedoMap;
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}

void Particle::useShader() {
	if(runAnimation) t = t + delta_t;

	shader->use();
	shader->setmat4(model, "model");
	shader->setmat4(cam->view, "view");
	shader->setmat4(cam->projection, "projection");
	shader->setInt(particleCount, "particleCount");
	shader->setFloat(t, "time");
	shader->setmat4(water_model, "water_model");
	shader->setFloat(t_diff, "water_time");

	albedoMap->use(GL_TEXTURE0);
	shader->setInt(0, "albedoMap");

	shader->setFloat(t_diff, "hitTime");
}



void Particle::draw() {
	glBindVertexArray(quadVAO);
	useShader();
	glDrawArraysInstanced(GL_TRIANGLES, 0, 12, particleCount);
	//glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindVertexArray(0);
}