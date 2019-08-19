#ifndef WATER_H
#define WATER_H
#include "Camera.h"
#include "RenderObject.h"
class Water : public RenderObject
{
private:
	std::vector<std::vector<glm::vec3>> vertices; //vertices for the grid
	int w, l; //w is width of vertices (left to right), l is length of vertices (up to down).
	float verticesSpace;
	float t; //time is changing
	std::vector< glm::vec3> triangles;
	VAO* vao;
	Shader* shader;
	Shader* testShader;
	const char* vertex = "waterVertex.txt";
	const char* frag = "waterFrag.txt";
	const char* geo = "waterGeo.txt";

	glm::mat4 model, view, proj;

	//construct triangles from vertices
	void addTriangles();
	void addTrianglesToBuffer();
	void createShader(const char* vertex, const char* frag);
	void useShader();

	void waterLogic();
public:
	Water() {}
	Water(int w, int l, float s = 0.5f);
	~Water();
	void draw();
};

#endif

