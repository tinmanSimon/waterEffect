#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <Simon/simon_func.h>

//#include "Camera.h"

class Shader
{
	unsigned int vertexShader;
	unsigned int fragShader;
	unsigned int geoShader;
	unsigned int shaderProgram;

	//file names for vertex and frag shaders
	const char* vertexName;
	const char* fragName;
	const char* geoName;

	void shaderInit();
public:

	//vertex is the filename of vertex shader
	//frag is the filename of frag shader
	Shader() {}
	Shader(const char* vertex, const char* frag, const char* geo = nullptr);
	Shader(std::string vertex, std::string frag, std::string geo = "");
	~Shader();

	void use();
	void setmat4(glm::mat4&, const char*);
	void setInt(int i, const char*);
	void setFloat(float f, const char*);
	void setVec3(float valx, float valy, float valz, const char* attrName);
};

extern std::vector<Shader*> shaders;

#endif
