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
	std::vector< glm::vec3> triangles;
	long trianglesCount = 0;
	VAO* vao;
	Shader* shader;
	Shader* testShader;
	const char* vertex = "waterVertex.txt";
	const char* frag = "waterFrag2.txt";
	const char* geo = "waterGeo.txt";

	glm::mat4 model, view, proj;

	//construct triangles from vertices
	void addTriangles();
	void addTrianglesToBuffer();
	void createShader(const char* vertex, const char* frag);
	void useShader();
	void initShader();
	void waterLogic();

	//normal mapping
	Texture* normalText;
	void useTextures();
	void initTextures();
	void releaseResources();
public:
	Water() {}
	Water(int w, int l, float s = 0.5f);
	~Water();
	void draw();
	static bool geometry;
};

#endif

