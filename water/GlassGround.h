#ifndef GLASSGROUND_H
#define GLASSGROUND_H
#include "RenderObject.h"
#include "VAO.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
class GLASSGROUND : public RenderObject
{
private:
	float altitude;
	float edge;
	std::vector< glm::vec3> triangles;
	long traingleSize;

	VAO* vao;
	Shader* shader;
	Texture* text;
	Texture* albedoMap;
	const char* vertex = "glassGroundVertex.txt";
	const char* frag = "glassGroundFrag.txt";
	const char* text_file_name = "transparent_glass.png";
	void addTrianglesToBuffer();
	void useShader(Shader* shader, Texture* text);
public:
	GLASSGROUND() {}
	GLASSGROUND(float a, float e = 40.0f);
	~GLASSGROUND();
	void draw();
	float getAltitude();
	float getEdge();
};
#endif

