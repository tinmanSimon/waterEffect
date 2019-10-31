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



extern Window* window;

using namespace std;
using namespace glm;

int main() {
	float width = 3000;
	float height = 1500;
	char w_name[100] = "waterEffect";
	
	window = new Window(w_name, width, height);
	if (window->init() != 0) {
		cout << "window init failed in main.cpp!" << endl;
		return -1;
	}
	window->loop();
	free(window);
	
	
	return 0;
}