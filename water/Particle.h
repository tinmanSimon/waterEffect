#ifndef PARTICLE_H
#define PARTICLE_H

#include "RenderObject.h"
#include "VAO.h"
#include "Camera.h"
#include "Shader.h"
class Particle : public RenderObject {
	int ID;
	VAO* vao;
	Shader* shader;
	const char* vertex = "particleVertex.txt";
	const char* frag = "particleFrag.txt";
	unsigned int quadVBO; 
	unsigned int quadVAO;
	void useShader(int i);
	glm::mat4 model, view, proj;
	std::vector<glm::mat4> models;
	void initShader();
	Texture* albedoMap;
	float t;
	bool hitWater;
	std::vector<bool> hitWaters;
	float hit_time;
	std::vector <float> hit_times;
	glm::vec3 velocity;
	std::vector <glm::vec3> velocities;
public:
	Particle(int i);
	~Particle();
	void draw();
	void reset_time();
	void it_hits_water(glm::vec3 pos, glm::vec3 v, int i);
	bool hits_water();
};

#endif