#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include "VAO.h"
#include "Shader.h"
#include "Camera.h"
#include "RenderObject.h"
#include "Texture.h"

class Sphere : public RenderObject {
	int ID;
	glm::vec3 pos;
	float radius;
	float mass;
	int slices, layers;

	VAO* vao = NULL;
	Shader* shader = NULL;

	glm::mat4 model, view, proj;

	const char* vertex = "sphereVertex.txt";
	const char* frag = "sphereFrag.txt";
	long pointSize;
	long triangleIndiciesSize;
	long uvSize;

	Texture* albedoMap;
	Texture* normalMap;
	Texture* metallicMap;
	Texture* roughnessMap;
	Texture* aoMap;


public:
	static int ID_count;
	//Sphere();
	Sphere(glm::vec3 position, float r, int sli, int lay, float m = 1.0f);
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
	void draw();
	void drawShadow(Shader* s); //means use s to render not using the default water shader
	void update(glm::mat4 m);

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::u32vec3> trianglesIndices;
	std::vector<glm::vec2> uvs;

	float roughness = 0.5f;
	float metallic = 0.5f;

	void useShader();
	void setTextures();
	void useTextures();

	glm::vec3 velocity;
	void sphere_translate(glm::vec3 v);
	glm::vec3 getWorldPos();
	float getRadius();
	Texture* getAlbedoTexture();
};

extern std::vector<Sphere*> spheres;

#endif