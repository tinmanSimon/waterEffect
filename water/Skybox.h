#ifndef SKYBOX_H
#define SKYBOX_H
#include <vector>
#include "RenderObject.h"
#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "Camera.h"
class Skybox : public RenderObject {
	Texture* text;
	Shader* shader;
	VAO* vao;
	const char* vertex = "skyboxVertex.txt";
	const char* frag = "skyboxFrag.txt";
public:
	Skybox();
	~Skybox();
	void draw();
};

#endif