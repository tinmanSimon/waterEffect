#ifndef DRAWER_H
#define DRAWER_H

#include "Camera.h"
#include "VAO.h"
#include "Shader.h"
#include "Sphere.h"
#include "Texture.h"
#include "Water.h"
#include "RenderObject.h"

extern bool pause;
extern int skyboxes;

/*
This is the class that deals with logic and drawings
*/
class Drawer
{
private:
	static std::vector<RenderObject*> renderObjects;
	//static std::vector<Water*> renderObjects;
public:
	Drawer();
	~Drawer();
	static void drawerinit();
	static void draw();
	static void freeRenderObjects();
};



void logic();

void cleanup();

#endif