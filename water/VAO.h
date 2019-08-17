#ifndef VAO_H
#define VAO_H


#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <vector>
#include <iostream>


class VAO
{
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	bool weNeedEBO;
public:
	
	//VAO pushes itself to VAOS, so no need to push it manually
	VAO() {}
	VAO(bool needEBO);
	~VAO();

	//usage is GL_STATIC_DRAW by default
	void bufferData(void* data, GLsizeiptr size, void* ebodata = NULL, GLsizeiptr ebosize = 0, GLenum = GL_STATIC_DRAW);
	void bufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data, GLenum target = GL_ARRAY_BUFFER);

	//pointer is offset. Also, by default type = GL_FLOAT, and normalized = GL_FALSE.
	//stride is like this: 5*sizeof(float). 
	//pointer is like this: (void*)(3*sizeof(float))
	void addAttribute(GLuint index,
		GLint size,
		GLsizei stride,
		const GLvoid* pointer,
		GLenum type = GL_FLOAT,
		GLboolean normalized = GL_FALSE);

	void use();
};

extern std::vector<VAO*> VAOS;


#endif