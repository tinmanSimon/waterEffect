#include "Particle.h"
using namespace glm;

/*
float particleVertices[] = {
	-0.05f,  0.0f, 0,
	 0.05f, 0.f, 0,
	0.f, 0.05f, 0
};
*/


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


Particle::Particle() {
	//preprocessing
	forUp(i, 36) particleVertices[i] *= 0.01;

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	shader = new Shader(vertex, frag);
	model = mat4(1);
}


Particle::~Particle() {
	delete shader;
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}

void Particle::useShader() {
	shader->use();
	shader->setmat4(model, "model");
	shader->setmat4(cam->view, "view");
	shader->setmat4(cam->projection, "projection");
}



void Particle::draw() {
	glBindVertexArray(quadVAO);
	useShader();
	glDrawArraysInstanced(GL_TRIANGLES, 0, 12, 100);
	//glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindVertexArray(0);
}