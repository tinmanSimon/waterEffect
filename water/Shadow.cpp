#include "Shadow.h"

using namespace glm;
extern vec3 sunLightDir;
extern Window* the_window;
mat4 lightSpaceMatrix;
mat4 lightProjection;
mat4 lightView;


Shadow::Shadow(float w, float h) : width{ w }, height{ h } {
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	depthShader = new Shader(vertex, frag);
}


Shadow::~Shadow() {
	delete depthShader;
}

void Shadow::use() {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	float n = -10, f = 20;
	float side = 10;
	lightProjection = ortho(-side, side, -side, side, n, f);
	//vec3 lightPos = 1000.0f * sunLightDir;
	lightView = lookAt(10.0f * sunLightDir, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;

	depthShader->setmat4(lightSpaceMatrix, "lightSpaceMatrix");
	//todo useShader();
}

void Shadow::changeBack() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, the_window->width, the_window->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	//todo useShader();
}

Shader* Shadow::getShader() {
	return depthShader;
}