#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H
#include <iostream>
#include "Shader.h"
// Interface for all rendering objects.
class RenderObject
{
public:
	RenderObject();
	virtual ~RenderObject();
	virtual void draw() { std::cout << "Error! RenderObject's virtual draw function is called!"; }
	virtual void drawShadow(Shader*) { std::cout << "Error! RenderObject's virtual drawShadow function is called!"; }
};
#endif
