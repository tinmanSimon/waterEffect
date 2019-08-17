#include "VAO.h"

using namespace std;
vector<VAO*> VAOS;

VAO::VAO(bool needEBO) : ebo{ 0 }, weNeedEBO{needEBO}
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	if (weNeedEBO) glGenBuffers(1, &ebo);
}


VAO::~VAO()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	if(weNeedEBO) glDeleteBuffers(1, &ebo);
}

void VAO::bufferData(void* data, GLsizeiptr size, void* ebodata, GLsizeiptr ebosize, GLenum usage) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);

	if (ebodata != NULL) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebosize, ebodata, usage);
	}
}

void VAO::bufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data, GLenum target) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(target, offset, size, data);
}

void VAO::addAttribute(GLuint index,
	GLint size,
	GLsizei stride,
	const GLvoid* pointer,
	GLenum type,
	GLboolean normalized) {
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
}

void VAO::use() {
	glBindVertexArray(vao);
}

