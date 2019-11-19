#include "Sphere.h"


using namespace std;
using namespace glm;

std::vector<Sphere*> spheres;
extern vec3 sunLightDir;

/*
Sphere::Sphere()
{
}
*/

static float PI_S = 3.1415926;

extern vector<Sphere*> render_spheres;

Sphere::Sphere(glm::vec3 position, float r, int sli, int lay, float m) : ID{ Sphere::ID_count++ }, pos{ vec3(0, 0, 0) }, radius{ r }, slices{ sli }, layers{ lay }, velocity{ vec3(0) }, mass{ m }, initialPos{ position } {

	//Step 1, generate all the points.
	//init points
	int pointCount = 0;
	int normalCount = 0;
	int uvCount = 0;
	points.resize(slices* (layers - 1) + 2);
	normals.resize(slices* (layers - 1) + 2);
	uvs.resize(slices* (layers - 1) + 2);

	//generate point at the top
	points[pointCount++] = vec3(pos.x, pos.y + radius, pos.z);
	normals[normalCount++] = vec3(0.0f, 1.0f, 0.0f);
	uvs[uvCount++] = vec2((0.5f + atan2(0, 0) / (2*PI_S)), 0.5f - asin(-1) / PI_S);

	//generate all the points in the middle layers
	for (float i = 1.0f; i < layers; ++i) {
		float N = layers;
		float L = radius * sqrt(1 - (N - 2 * i) * (N - 2 * i) / (N * N));
		float theta = 360.0f / ((float)slices);
		for (int j = 0; j < slices; ++j) {
			float curtheta = theta * (float)j;
			float x, y, z;
			x = L * (float)cos(radians(curtheta));
			y = (N - 2 * i) * radius / N;
			z = L * (float)sin(radians(curtheta));
			points[pointCount++] = vec3(x, y, z) + pos;
			normals[normalCount++] = vec3(x, y, z);
			vec3 the_vec = normalize(vec3(0) - (vec3(x, y, z)+ pos));
			uvs[uvCount++] = vec2((0.5f + atan2(the_vec.z, the_vec.x) / (2*PI_S)), 0.5f - asin(the_vec.y) / PI_S);
		}
	}

	//generate point at the bottom
	points[pointCount++] = vec3(pos.x, pos.y - radius, pos.z);
	normals[normalCount++] = vec3(0.0f, -1.0f, 0.0f);
	uvs[uvCount++] = vec2((0.5f + atan2(0, 0) / (2*PI_S)), 0.5f - asin(1) / PI_S);

	//forUp(i, uvs.size()) cout<<glm::to_string(uvs[i])<<endl;

	//Step 2, generate all the indices for triangles
	int traingleCount = 0;
	trianglesIndices.resize(2 * slices* layers);

	//generate triangles indices from top points to the 1st layers
	for (int i = 0; i < slices; ++i) {
		if(i == slices - 1) trianglesIndices[traingleCount++] = u32vec3(0, i + 1, 1);
		else trianglesIndices[traingleCount++] = u32vec3(0, i + 1, i + 2);
	}

	//generate triangles indices in the middle
	for (int i = 1; i < layers - 1; ++i) {
		int startPoint = (i - 1) * slices + 1;
		int nextLayerSP = i * slices + 1;
		for (int j = 0; j < slices; ++j) {
			if (j == slices - 1) {
				trianglesIndices[traingleCount++] = u32vec3(startPoint + j, nextLayerSP + j, nextLayerSP);
				trianglesIndices[traingleCount++] = u32vec3(nextLayerSP, startPoint, startPoint + j);
			}
			else {
				trianglesIndices[traingleCount++] = u32vec3(startPoint + j, nextLayerSP + j, nextLayerSP + j + 1);
				trianglesIndices[traingleCount++] = u32vec3(nextLayerSP + j + 1, startPoint + j + 1, startPoint + j);
			}
		}
	}

	//generate triangles indices from bottom points to the last layers
	int startPoint = points.size() - 1 - slices;
	int endPoint = points.size() - 1;
	for (int i = 0; i < slices; ++i) {
		if (i == slices - 1) trianglesIndices[traingleCount++] = u32vec3(startPoint + i, endPoint, startPoint);
		else trianglesIndices[traingleCount++] = u32vec3(startPoint + i, endPoint, startPoint + i + 1);
	}

	createVAO();
	bufferData(2);
	createShader(vertex, frag);
	pointSize = points.size();
	triangleIndiciesSize = trianglesIndices.size();
	uvSize = uvs.size();
	points.clear();
	trianglesIndices.clear();
	uvs.clear();

	vec3 trans = position - pos;
	model = translate(mat4(1), trans);
	setTextures();
}


Sphere::~Sphere() {
	delete vao;
	delete shader;
}

void Sphere::createVAO() {
	vao = new VAO(true);
}

void Sphere::bufferData(int dataType) {
	if (vao == NULL) createVAO();

	//points
	if (dataType == 1) {
		vao->bufferData(NULL, points.size() * sizeof(vec3) + normals.size() * sizeof(vec3));
		vao->bufferSubData(0, points.size() * sizeof(vec3), &points[0]);
		vao->bufferSubData(points.size() * sizeof(vec3), normals.size() * sizeof(vec3), &normals[0]);
		vao->addAttribute(0, 3, 3 * sizeof(float), 0);
		vao->addAttribute(2, 3, 3 * sizeof(float), (void*)(points.size() * sizeof(vec3)));
	}

	//triangles
	else if (dataType == 2) {
		vao->bufferData(NULL, points.size() * sizeof(vec3) + normals.size() * sizeof(vec3) + uvs.size() * sizeof(vec2), &trianglesIndices[0], trianglesIndices.size()*sizeof(u32vec3));
		vao->bufferSubData(0, points.size() * sizeof(vec3), &points[0]);
		vao->bufferSubData(points.size() * sizeof(vec3), normals.size() * sizeof(vec3), &normals[0]);
		vao->bufferSubData(points.size() * sizeof(vec3) + normals.size() * sizeof(vec3), uvs.size() * sizeof(vec2), &uvs[0]);
		vao->addAttribute(0, 3, 3 * sizeof(float), 0);
		vao->addAttribute(1, 3, 3 * sizeof(float), (void*)(points.size() * sizeof(vec3)));
		vao->addAttribute(2, 2, 2 * sizeof(float), (void*)(points.size() * sizeof(vec3) + normals.size() * sizeof(vec3)));
	}
	else cout << "ERROR! wrong data type for bufferdata! datatype = " << dataType << endl;
}

void Sphere::bufferData(VAO* v, int dataType) {
	if(vao != NULL) delete vao;
	vao = v;
	bufferData(dataType);
}

void Sphere::createShader(const char* vertex, const char* frag) {
	shader = new Shader(vertex, frag);
	//shaders.push_back(shader);
}

void Sphere::setShader(Shader* s) {
	if(shader != NULL) delete shader;
	shader = s;
}

void Sphere::useShader(){
	view = cam->view;
	proj = cam->projection;

	shader->use();
	shader->setmat4(model, "model");
	shader->setmat4(view, "view");
	shader->setmat4(proj, "projection");
	shader->setVec3(cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z, "camPos");
	shader->setVec3(sunLightDir.x, sunLightDir.y, sunLightDir.z, "lightPosition");

	shader->setInt(10, "shadowMap"); //10 is for shadow map only

	shader->setInt(render_spheres.size(), "spheres.size");
	forUp(i, render_spheres.size()) {
		Sphere* curSphere = render_spheres[i];
		string tmp = "spheres.sphere_pos[" + to_string(i) + "]";
		shader->setVec3(curSphere->getWorldPos(), tmp.c_str());
		tmp = "spheres.sphere_Radius[" + to_string(i) + "]";
		shader->setFloat(curSphere->getRadius(), tmp.c_str());
	}
	//cout << "ID:" << ID << endl;
	shader->setInt(ID, "ID");
}

void Sphere::draw(int drawType) {
	if (vao == NULL) {
		cout << "ERROR! vao is null but sphere tries to draw!" << endl;
		return;
	}
	else if (shader == NULL) {
		cout << "ERROR! shader is null but sphere tries to draw! Assign sphere's shader!" << endl;
		return;
	}
	vao->use();
	useShader();
	useTextures();
	if (drawType == 1) glDrawArrays(GL_POINTS, 0, points.size());
	else if(drawType == 2) glDrawElements(GL_TRIANGLES, 3*triangleIndiciesSize, GL_UNSIGNED_INT, 0);
}

void Sphere::drawShadow(Shader* s) {
	if (vao == NULL) {
		cout << "ERROR! vao is null but sphere tries to draw!" << endl;
		return;
	}
	else if (shader == NULL) {
		cout << "ERROR! shader is null but sphere tries to draw! Assign sphere's shader!" << endl;
		return;
	}
	vao->use();
	s->setmat4(model, "model");
	s->use();
	useTextures();
	glDrawElements(GL_TRIANGLES, 3 * triangleIndiciesSize, GL_UNSIGNED_INT, 0);
}


void Sphere::draw(){
	draw(2);
}

void Sphere::update(mat4 m) {
	model = m;
	shader->setVec3(cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z, "camPos");
	shader->setFloat(metallic, "metallic");
	shader->setFloat(roughness, "roughness");
	shader->setVec3(5.0f, 5.0f, 5.0f, "lightPosition");
	shader->setVec3(300.0f, 300.0f, 300.0f, "lightColor");
	float t = abs(sin(glfwGetTime()));
	shader->setVec3(t, 1.0f-t, 0.0f, "albedo");
	//shader->setVec3(0.5f,0.0f, 0.0f, "albedo");
	shader->setFloat(1.0f, "ao");
}

void Sphere::setTextures(){
	albedoMap = new Texture("copper-rock1-alb.png");
	aoMap = new Texture("copper-rock1-ao.png");
	normalMap = new Texture("copper-rock1-normal.png");
	metallicMap = new Texture("copper-rock1-metal.png");
	roughnessMap = new Texture("copper-rock1-rough.png");
	

	shader->use();
	shader->setInt(0, "albedoMap");
	shader->setInt(1, "normalMap");
	shader->setInt(2, "metallicMap");
	shader->setInt(3, "roughnessMap");
	shader->setInt(4, "aoMap");
}

void Sphere::useTextures(){
	albedoMap->use(GL_TEXTURE0);
	normalMap->use(GL_TEXTURE1);
	metallicMap->use(GL_TEXTURE2);
	roughnessMap->use(GL_TEXTURE3);
	aoMap->use(GL_TEXTURE4);
}

void Sphere::sphere_translate(glm::vec3 v) {
	model = translate(model, v);
}

vec3 Sphere::getWorldPos() {
	return vec3(model * vec4(pos, 1));
}

float Sphere::getRadius() {
	return radius;
}

Texture* Sphere::getAlbedoTexture() {
	return albedoMap;
}

int Sphere::getID() {
	return ID;
}

void Sphere::reset(vec3 pos) {
	vec3 trans = pos;
	model = translate(mat4(1), trans);
	velocity = vec3(0);
}

void Sphere::reset() {
	vec3 trans = initialPos;
	model = translate(mat4(1), trans);
	velocity = vec3(0);
}