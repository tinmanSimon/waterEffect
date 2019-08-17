#include "Shader.h"


using namespace std;
using namespace glm;

vector<Shader*> shaders;

void compile(const char* fileName, unsigned int& shader, GLenum shaderType) {
	string bufferstr = fileToStr(fileName);
	const char* vertexShaderSource = bufferstr.c_str();
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &vertexShaderSource, NULL);
	glCompileShader(shader);
	int  success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR: Vertex Shader compilation failed!\n" << infoLog << endl;
	}
}

void Shader::shaderInit() {
	//compile vertex shader
	compile(vertexName, vertexShader, GL_VERTEX_SHADER);

	//compile fragment shader
	compile(fragName, fragShader, GL_FRAGMENT_SHADER);

	//link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	int success = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR: Shader program linking failed!\n" << infoLog << endl;
	}

	//unlink shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

Shader::Shader(const char* vertex, const char* frag) {
	vertexName = vertex;
	fragName = frag;
	shaderInit();

}

Shader::Shader(string vertex, string frag)
{
	if (vertex.length() == 0 || frag.length() == 0) {
		cout << "ERROR: invalid shader file names!\n" << endl;
	}
	else {
		vertexName = vertex.c_str();
		fragName = frag.c_str();
		shaderInit();
	}

}


Shader::~Shader()
{
}

void Shader::use() {
	glUseProgram(shaderProgram);
}

void Shader::setmat4(glm::mat4& val, const char* attrName) {
	use();
	int loc = glGetUniformLocation(shaderProgram, attrName);
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(val));
}

void Shader::setInt(int i, const char* targetname) {
	use();
	glUniform1i(glGetUniformLocation(shaderProgram, targetname), i);
}

void Shader::setFloat(float f, const char* targetname) {
	use();
	glUniform1f(glGetUniformLocation(shaderProgram, targetname), f);
}

void Shader::setVec3(float valx, float valy, float valz, const char* attrName) {
	use();
	int loc = glGetUniformLocation(shaderProgram, attrName);
	glUniform3f(loc, valx, valy, valz);
}