#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "VAO.h"
#include "Shader.h"
#include "Camera.h"

class Sphere
{
	glm::vec3 pos;
	float radius;
	int slices, layers;

	VAO* vao = NULL;
	Shader* shader = NULL;

	glm::mat4 model, view, proj;


public:
	//Sphere();
	Sphere(glm::vec3 position, float r, int sli, int lay);
	~Sphere();

	//can create vao for itself, or it can be added by outside
	void createVAO();

	//datatype == 1 then it's points, 3 then it's lines, 2 then it's triangles
	void bufferData(int datatype);
	void bufferData(VAO* v, int);
	void createShader(const char* vertex, const char* frag);
	void setShader(Shader* s);
	//drawType == 1 then it's points, 3 then it's lines, 2 then it's triangles
	void draw(int drawType);
	void update(glm::mat4 m, glm::mat4 v, glm::mat4 p);

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::u32vec3> trianglesIndices;

	float roughness = 0.5f;
	float metallic = 0.5f;
};

extern std::vector<Sphere*> spheres;

#endif