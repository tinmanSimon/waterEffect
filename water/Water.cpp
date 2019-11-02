#include "Water.h"

using namespace std;
using namespace glm;

bool Water::geometry = true;
extern float t;

void Water::addTriangles() {
	triangles.resize((l - 1) * (w - 1) * 6 * 2);
	int triangleCount = 0;
	forUp(i, vertices.size() - 1) {
		forUp(j, vertices[i].size() - 1) {
			triangles[triangleCount++] = vertices[i][j];
			//triangles[triangleCount++] = vec3(0.0f, 0.0f, 0.0f);
			triangles[triangleCount++] = vertices[i][j+1];
			//triangles[triangleCount++] = vec3(1.0f, 0.0f, 0.0f);
			triangles[triangleCount++] = vertices[i+1][j+1];
			//triangles[triangleCount++] = vec3(1.0f, 1.0f, 0.0f);

			
			triangles[triangleCount++] = vertices[i][j];
			//triangles[triangleCount++] = vec3(0.0f, 0.0f, 0.0f);
			triangles[triangleCount++] = vertices[i + 1][j + 1];
			//triangles[triangleCount++] = vec3(1.0f, 1.0f, 0.0f);
			triangles[triangleCount++] = vertices[i + 1][j];
			//triangles[triangleCount++] = vec3(0.0f, 1.0f, 0.0f);
			
		}
	}
}

void Water::addTrianglesToBuffer() {
	vao->bufferData(&triangles[0], triangles.size()*sizeof(vec3));
	vao->addAttribute(0, 3, 3*sizeof(float), 0);
	//vao->addAttribute(1, 2, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Water::createShader(const char* vertex, const char* frag) {
	//shader = Shader(vertex, frag);
}

void Water::waterLogic() {
	model = translate(mat4(1.0f), vec3(0, -1.0f, 0));
	view = cam->view;
	proj = cam->projection;

	//Todo change t to real time simulation
	//t = glfwGetTime();
	//t = 10;
}

void Water::useTextures() {
	shader->setInt(0, "normalMap");
	normalText->use(GL_TEXTURE0);
}

void Water::initTextures() {
	normalText = new Texture("water_normal.jpg");
	//normalText = new Texture("water.jpg");
}

void Water::releaseResources() {
	vertices.clear();
	triangles.clear();
}

Water::Water(int width, int length, float s) : 
	//note width + 1 because we want start from 0 and end up with w, so w + 1 lines. Same for l.
	w{ width + 1 }, l{ length + 1 }, verticesSpace{ s }, model{ mat4(1) } 
{
	assert(width > 0);
	assert(length > 0);
	assert(s > 0);

	//construct vao and shader
	vao = new VAO(false);
	initShader();

	//construct vertices. 
	vertices.resize(l);
	forUp(i, l) { 
		float z = (float)(i - (l / 2)) * verticesSpace;
		vertices[i].resize(w, vec3(1,0,z));
		forUp(j, vertices[i].size()) {
			vertices[i][j].x = (float) (j - (w / 2)) * verticesSpace ;
		}
	}
	addTriangles();
	trianglesCount = triangles.size();
	addTrianglesToBuffer();
	//initTextures();
	releaseResources();
}

void Water::useShader() {
	//config for vertex shader
	shader->use();
	shader->setmat4(model, "model");
	shader->setmat4(view, "view");
	shader->setmat4(proj, "projection");
	shader->setFloat(t, "t");

	//config for frag shader
	shader->setVec3(0, 0.412, 0.58, "albedo");
	shader->setFloat(0.5, "metallic");
	shader->setFloat(0.5, "roughness");
	shader->setFloat(0.5, "ao");
	shader->setVec3(0, 0.5, 0.8, "lightPosition");
	shader->setVec3(1, 1, 0, "lightColor");
	shader->setVec3(cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z, "camPos");
}

void Water::initShader() {
	//shader = new Shader(vertex, frag, geo);
	shader = new Shader(vertex, frag);

	//init vertex shader
	string tmp;
	int waveSize = 30;
	shader->setInt(waveSize, "waveSize");
	//vec2 dirct = vec2(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f));
	forUp(i, waveSize) {
		vec2 dirct = vec2(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f));
		vec2 direction = vec2(dirct.x + randFloat(-0.4f, 0.4f), dirct.y + randFloat(-0.4f, 0.4f));
		float A = randFloat(0, 1/(float)waveSize); //waves amplitude
		float speed = randFloat(0.0f, 0.8f); //waves speed
		float wavelength = randFloat(1.0f, 3.0f); 
		float w = 2 * PI_SIMON / wavelength;
		float Q = randFloat(0, 1);
		float phase_constant = speed * 2 * PI_SIMON / wavelength;
		tmp = "waves.Ds[" + to_string(i) + "]";
		shader->setVec2(direction, tmp.c_str());
		tmp = "waves.As[" + to_string(i) + "]";
		shader->setFloat(A, tmp.c_str());
		tmp = "waves.speeds[" + to_string(i) + "]"; 
		shader->setFloat(speed, tmp.c_str());
		tmp = "waves.wavelengths[" + to_string(i) + "]"; 
		shader->setFloat(wavelength, tmp.c_str());
		tmp = "waves.ws[" + to_string(i) + "]"; //ws
		shader->setFloat(w, tmp.c_str());
		tmp = "waves.Qs[" + to_string(i) + "]"; //Qs
		shader->setFloat(Q, tmp.c_str());
		tmp = "waves.phase_constants[" + to_string(i) + "]"; //Qs
		shader->setFloat(phase_constant, tmp.c_str());
	}
	
}

Water::~Water()
{
	free(vao);
	free(shader);
	if(normalText != NULL) free(normalText);
}

void Water::draw() {
	waterLogic();
	vao->use();
	useShader();
	//useTextures();
	if(geometry) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, trianglesCount);
}
