#include "Window.h"
#include <Simon/simon_func.h>
#include <STB_IMG/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>



extern Window* the_window;

using namespace std;
using namespace glm;

int main() {
	float width = 3000;
	float height = 1500;
	char w_name[100] = "waterEffect";
	
	the_window = new Window(w_name, width, height);
	if (the_window->init() != 0) {
		cout << "window init failed in main.cpp!" << endl;
		return -1;
	}
	the_window->loop();
	free(the_window);
	
	
	return 0;
}