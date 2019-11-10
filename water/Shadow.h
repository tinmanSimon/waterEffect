#ifndef SHADOW_H
#define SHADOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "VAO.h"
#include "Shader.h"
#include "Window.h"

//GL_TEXTURE10 for shadow map only
class Shadow {
	unsigned int depthMapFBO;
	unsigned int depthMap;
	float width, height;
	Shader* depthShader;
	const char* vertex = "depthVertex.txt";
	const char* frag = "depthFrag.txt";
public:
	Shadow() {}
	Shadow(float w, float h);
	~Shadow();
	void use();
	void changeBack();
	Shader* getShader();
};

#endif