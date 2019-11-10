#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "VAO.h"
#include "Shader.h"

class FrameBuffer {
private:
	unsigned int framebuffer;
	unsigned int texColorBuffer;
	unsigned int rbo;
	float width, height;

	const char* vertex = "frameBufferVertex.txt";
	const char* frag = "frameBufferFrag.txt";

	VAO* vao;
	Shader* shader;
public:
	FrameBuffer() {}
	FrameBuffer(float width, float height);
	~FrameBuffer();
	void genColorAndRBO(float width, float height);
	void useFrameBuffer(glm::vec3 clearColor = glm::vec3(0.1f, 0.1f, 0.1f));
	void changeBackToDefaultBufferAndDraw(glm::vec3 clearColor = glm::vec3(1, 1, 1));
};

#endif