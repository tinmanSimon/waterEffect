#include "FrameBuffer.h"
using namespace std;
using namespace glm;

int quads_size = 24;
float quads[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

void addQuadsToBuffer(VAO* vao) {
	vao->bufferData(&quads[0], quads_size * sizeof(float));
	vao->addAttribute(0, 2, 4 * sizeof(float), 0);
	vao->addAttribute(1, 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

FrameBuffer::FrameBuffer(float w, float h) : width{ w }, height{ h } {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	genColorAndRBO(width, height);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "Error! Framebuffer NOT complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	vao = new VAO(false);
	addQuadsToBuffer(vao);
	shader = new Shader(vertex, frag);
	shader->setInt(0, "texture0");
	shader->setInt(1, "depthBuffer");
}


FrameBuffer::~FrameBuffer() {
	delete vao;
	delete shader;
}

void FrameBuffer::genColorAndRBO(float width, float height) {
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	/*
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	*/

	/*
	glGenTextures(1, &stencil);
	glBindTexture(GL_TEXTURE_2D, stencil);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencil, 0);

	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
	*/


	
	glGenTextures(1, &rbo);
	glBindTexture(GL_TEXTURE_2D, rbo);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rbo, 0);
	

	//depth buffer
	/*
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
	*/

}

void FrameBuffer::useFrameBuffer(vec3 clearColor) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::changeBackToDefaultBufferAndDraw(vec3 clearColor) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	shader->use();
	vao->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rbo);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}