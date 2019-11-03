#ifndef WINDOW_H
#define WINDOW_H

#include "Drawer.h"
#include "Sphere.h"
#include <fstream>
#include <iostream>



class Window
{
	char* name;

	GLFWwindow* window;



public:
	float width;
	float height;

	Window(char* window_name, float _width, float _height);
	~Window();

	//init including z buffer, removing cursor.
	//returns 0 if nothing goes wrong, otherwise -1.
	int init();

	//the main loop
	void loop();


};


#endif