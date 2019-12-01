#include "Drawer.h"

using namespace std;
using namespace glm;

bool pause = false;

unsigned int framebuffer; //for fbo test only
unsigned int intermediateFBO; //for fbo test only
unsigned int texColorBuffer;
unsigned int rbo;

Texture* tForFbo;
Texture* tForIFbo;
VAO* msaaVAO;
Shader* msaaShader;
Sphere* player;
GLASSGROUND* glass;
Particle* particle;

int enemy_count = 10;
int render_count = 11;
extern const int original_render_count = 11;

bool jump = false;

vector<Sphere*> render_spheres;
vector<Particle*> render_particles;

vector<RenderObject*> Drawer::renderObjects;
//vector<Water*> Drawer::renderObjects;

float g_const = 13.0f;
float t; //time is changing
float delta_t;
bool runAnimation = false;
float threshold = 0.1f;
float energyConsumptionRate = 0.75f;

FrameBuffer* fbo;
extern Window* the_window;
extern vec3 clearColor;

Shadow* shadowmap;

int Sphere::ID_count;


extern int gameMode;

bool slow_motion_enabled = true;
float time_speed = 1.0f;
float min_time_speed = 0.1f;
float max_velocity = 10.0f;
float velocity_step = 0.1f;
float velocity_dash = 30.0f;
float jump_range = 0.5f;
float control_range = 0.1f;
vec3 jump_velocity = vec3(0, 7, 0);
float difficulty = 1.0f;

extern float fogDistance;
float fogChange = 10.0f;

bool useFrameBuffer = true;


extern bool enable_blur;
extern float blur_density;
extern float blured;
extern float blur_dismantle_speed;
extern float blur_limit;

int mode = 0; //0 normal mode. 1 skybox mode.
bool particleDemo = false;

Drawer::Drawer(){
}


Drawer::~Drawer() {
	for (auto obj : renderObjects) delete obj;
	delete fbo;
	delete shadowmap;
}


void Drawer::drawerinit() {
	//init simone func
	initRand();

	//init camera
	cam = new Camera();

	//init time
	t = glfwGetTime();

	//init skybox
	renderObjects.push_back(new Skybox());

	//init water
	Water::geometry = false;
	glass = new GLASSGROUND(4.0f, 40.0f);
	renderObjects.push_back(new Water(200, 200, 0.5f));
	//renderObjects.push_back(new WATERGROUND(-2.0f));
	//renderObjects.push_back(new WATERGROUND(-200.0f));

	//init spheres
	Sphere::ID_count = 0;

	player = new Sphere(vec3(-0.6, 30, -0.6), 1, 40, 40);
	renderObjects.push_back(player);
	render_spheres.push_back(player);
	forUp(i, enemy_count) {
		Sphere* tmp = new Sphere(vec3(0, i * 2.2 + 5.0, 0), 1, 40, 40);
		render_spheres.push_back(tmp);
		renderObjects.push_back(tmp);
	}

	//particle = new Particle();
	//renderObjects.push_back(particle);
	//forUp(i, enemy_count + 1) {
	particle = new Particle(enemy_count + 1);
	renderObjects.push_back(particle);
	//}

	renderObjects.push_back(glass);

	fbo = new FrameBuffer(the_window->width, the_window->height);
	shadowmap = new Shadow(1024, 1024);
}

bool sphereIsStatic(Sphere* s, GLASSGROUND* g) {
	float groundAltitude = g->getAltitude();
	float edge = g->getEdge();
	vec3 sphereOrigin = s->getWorldPos();
	if (sphereOrigin.x < -edge || sphereOrigin.x > edge || sphereOrigin.z < -edge || sphereOrigin.z > edge) return false;

	float diff = abs(sphereOrigin.y - groundAltitude) - s->getRadius();

	if (abs(diff) <= 0.02f && length(s->velocity) < 0.1f) {
		s->velocity = vec3(0);
		//cout << "sphereOrigin.y = " << sphereOrigin.y << ", radius: " << s->getRadius() << ", diff = " << diff << ", velocity length = " << length(s->velocity) << endl;
		return true;
	}
	//cout << "sphereOrigin.y = " << sphereOrigin.y << ", radius: " << s->getRadius() << ", diff = " << diff << ", velocity length = " << length(s->velocity) << endl;

	return false;

}

void updateSphere(Sphere* s) {
	if (sphereIsStatic(s, glass)) return;
	s->sphere_translate(s->velocity * delta_t + 0.5f * g_const * delta_t * delta_t * vec3(0, -1.0f, 0));
	s->velocity += g_const * delta_t * vec3(0, -1.0f, 0);
}

void collisionDetect(Sphere* s, GLASSGROUND* g) {
	bool itsStatic = sphereIsStatic(s, g);
	if (jump == false && s->getID() == player->getID() && itsStatic) return;
	else if(s->get_runAI() && itsStatic && s->getID() != player->getID()) s->velocity += jump_velocity;
	

	float groundAltitude = g->getAltitude();
	float edge = g->getEdge();
	vec3 sphereOrigin = s->getWorldPos();
	if (sphereOrigin.x < -edge || sphereOrigin.x > edge || sphereOrigin.z < -edge || sphereOrigin.z > edge) return;

	//now we only test its altitude
	float diff = abs(sphereOrigin.y - groundAltitude) - (s->getRadius() - threshold);
	//cout << "sphereOrigin.y = " << sphereOrigin.y << ", radius: " << s->getRadius() << ", diff = " << diff << endl;
	if (diff <= 0) {
		//we are hit
		//runAnimation = false;
		s->sphere_translate(vec3(0, -diff, 0));
		s->velocity = reflect(s->velocity, vec3(0, 1, 0));
		s->velocity = energyConsumptionRate * s->velocity;
		if (length(s->velocity) < 0.1f)s->velocity = vec3(0);

		if (s->getID() == player->getID() && jump && dot(s->velocity, vec3(0, 1, 0)) > 0) s->velocity += jump_velocity;
		else if (s->get_runAI() && s->getID() != player->getID() && dot(s->velocity, vec3(0, 1, 0)) > 0) s->velocity += jump_velocity;
		jump = false;
	}
}

void collisionDetect(Sphere* s1, Sphere* s2) {
	vec3 s1_Origin = s1->getWorldPos();
	vec3 s2_Origin = s2->getWorldPos();
	vec3 dist_vec = normalize(s1_Origin - s2_Origin);
	float dist = length(s1_Origin - s2_Origin) - (s1->getRadius() + s2->getRadius());

	//if they hit
	if (dist <= 0) {
		//slow motion
		if(slow_motion_enabled) time_speed = 4.0f;

		if (enable_blur) {
			if (s1->getID() == player->getID() || s2->getID() == player->getID()) {
				blured = 1.0f;
				blur_density = 100;
			}
		}

		s1->sphere_translate(dist_vec * abs(dist));
		float s1_velocity = dot(dist_vec, s1->velocity);
		float s2_velocity = dot(dist_vec, s2->velocity);
		vec3 s1_horizon_velocity = s1->velocity - s1_velocity * dist_vec;
		vec3 s2_horizon_velocity = s2->velocity - s2_velocity * dist_vec;
		s1->velocity = s2_velocity * dist_vec + s1_horizon_velocity;
		s2->velocity = s1_velocity * dist_vec + s2_horizon_velocity;
	}

}



void logic() {
	updateDeltaTime();

	delta_t = t;
	t = glfwGetTime();
	time_speed = max(time_speed, min_time_speed);
	delta_t = (t - delta_t) / time_speed;
	//cout << fogDistance << endl;

	if (runAnimation) {
		//blur
		if (blured > 0) {
			blur_density += blur_dismantle_speed;
			if (blur_density > blur_limit) blured = -1;
		}

		//fog
		if (fogDistance < (cam->n + 60) || fogDistance >(2.0f * cam->f)) {
			fogChange = -fogChange;
			fogDistance = min(max(fogDistance, cam->n + 61), 2.0f * cam->f - 1);
		}
		fogDistance += fogChange * delta_t;

		//update spheres
		forUp(i, render_count) {
			Sphere* s = render_spheres[i];
			//Particle* particle = render_particles[0];
			if (s->has_hit_water()) continue;

			//otherwise, it doesn't hit water and we update its location.
			updateSphere(s);
			for (int j = 0; j < render_spheres.size() - 1; ++j) 
				if(i != j && (!render_spheres[j]->has_hit_water())) collisionDetect(render_spheres[i], render_spheres[j]);
			
			//if (i + 1 < render_spheres.size()) collisionDetect(render_spheres[i], render_spheres[i + 1]);
			collisionDetect(render_spheres[i], glass);

			//AI of enemies
			if (s->get_runAI() && s->getID() != player->getID()) {
				vec3 dist_vec = normalize(player->getWorldPos() - s->getWorldPos());
				s->velocity += dist_vec * velocity_step * difficulty;
				if (length(s->velocity) > max_velocity) s->velocity = max_velocity * normalize(s->velocity);
			}

			//check if we hit the water, if yes then we render particles instead of spheres.
			if (s->getWorldPos().y + 1  - s->getRadius()< 1) {
				particle->it_hits_water(s->getWorldPos(), s->velocity, i);
				s->hit_water();
			}
		}
	}

	//update camera
	if (gameMode == 0) cam->update(player->getWorldPos(), 6);
	else if (gameMode == 1) cam->update();
	
}

void Drawer::draw() {
	//shadowmap->use();
	//forUp(i, renderObjects.size()) renderObjects[i]->drawShadow(shadowmap->getShader());
	//shadowmap->changeBack();

	if(useFrameBuffer) fbo->useFrameBuffer(clearColor);
	if(mode == 0) forUp(i, renderObjects.size()) renderObjects[i]->draw();
	else if(mode == 1) renderObjects[0]->draw();
	if (useFrameBuffer) fbo->changeBackToDefaultBufferAndDraw(clearColor);
}

void Drawer::freeRenderObjects() {
	forUp(i, renderObjects.size()) free(renderObjects[i]);
}

void cleanup() {
	for (int i = 0; i < VAOS.size(); ++i) {
		free(VAOS[i]);
	}
	for (int i = 0; i < shaders.size(); ++i) {
		free(shaders[i]);
	}
	for (int i = 0; i < globalTextures.size(); ++i) {
		free(globalTextures[i]);
	}
	for (int i = 0; i < spheres.size(); ++i) {
		free(spheres[i]);
	}

	Drawer::freeRenderObjects();

	free(cam);
}