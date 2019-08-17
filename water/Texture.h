#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <STB_IMG/stb_image.h>
#include <iostream>
#include <vector>


/*
1. Create Texture
2. assign texture attributes to shader
	e.g. shaders[0]->use();
		 shaders[0]->setInt(0, "texture1"); //texture1 is the attribute name in shaders[0]
3. Use Texture
	e.g. textures[0]->use(GL_TEXTURE0);
		 textures[1]->use(GL_TEXTURE1);
*/

extern int textureCount;

class Texture
{
	//for cubemaps
	void loadCubemap(std::vector<std::string>& faces);

public:
	//"texture" is ID of this texture,
	unsigned int id;
	int textCount;
	std::string type;
	std::string path;

	float w, h; //width and height for color buffer
	unsigned int framebuffer, rbo;

	Texture();

	//cubemap constructor. i == 1 means cubemap
	Texture(std::vector<std::string>& faces);

	//generate texture color buffer
	Texture(float, float, bool multisample = false);

	//reset function for color buffer
	void resetWidthHeight(float width, float height, bool multisample = false);

	//png's format is GL_RGBA, jpg is GL_RGB. internal_RGBA is the internal format, the third argument of glTexImage2D
	//wrapping_method == 1 means clamp
	Texture(const char*, std::string typestr = "", std::string pathstr = "", GLenum internal_RGBA = GL_RGB, int wrapping_method = 0);
	~Texture();

	void use(GLenum, GLenum form = GL_TEXTURE_2D);

	static void reverseImage();
};


extern std::vector<Texture*> globalTextures;

#endif 
