#ifndef WATERGROUND_H
#define WATERGROUND_H
#include "VAO.h"
#include "Shader.h"
#include "RenderObject.h"
#include "Camera.h"
class WATERGROUND : public RenderObject
{
private:
	float altitude;
	std::vector< glm::vec3> triangles;
	long traingleSize;

	VAO* vao;
	Shader* shader;
	const char* vertex = "waterGroundVertex.txt";
	const char* frag = "waterGroundFrag.txt";
	void addTrianglesToBuffer();
public:
	WATERGROUND(float a);
	WATERGROUND();
	~WATERGROUND();
	void draw();
	void drawShadow(Shader* s); //means use s to render not using the default water shader
};

#endif

