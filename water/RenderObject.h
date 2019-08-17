#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H
#include <iostream>
class RenderObject
{
public:
	RenderObject();
	~RenderObject();
	virtual void draw() { std::cout << "Error! RenderObject's virtual draw function is called!"; }
};
#endif
