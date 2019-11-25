#ifndef PARTICLE_H
#define PARTICLE_H

#include "RenderObject.h"
#include "VAO.h"
#include "Camera.h"
#include "Shader.h"
class Particle : public RenderObject {
	VAO* vao;
	Shader* shader;
	const char* vertex = "particleVertex.txt";
	const char* frag = "particleFrag.txt";
	unsigned int quadVBO; 
	unsigned int quadVAO;
	void useShader();
	glm::mat4 model, view, proj;
public:
	Particle();
	~Particle();
	void draw();
};

#endif