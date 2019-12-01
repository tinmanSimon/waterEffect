#include "Skybox.h"

using namespace std;
using namespace glm;

vector<std::string> marvelous{
		"bloody-marvelous_ft.jpg",
		"bloody-marvelous_bk.jpg",

		"bloody-marvelous_up.jpg",
		"bloody-marvelous_dn.jpg",

		"bloody-marvelous_rt.jpg",
		"bloody-marvelous_lf.jpg"
};

vector<std::string> clouds{
		"stormydays_ft.png",
		"stormydays_bk.png",

		"stormydays_up.png",
		"stormydays_dn.png",

		"stormydays_rt.png",
		"stormydays_lf.png"
};

vector<std::string> clouds2{
		"cloudtop_ft.png",
		"cloudtop_bk.png",

		"cloudtop_up.png",
		"cloud_down_looking.png",

		"cloudtop_rt.png",
		"cloudtop_lf.png"
};

float skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

Skybox::Skybox() {
	vao = new VAO(false);
	vao->bufferData(skyboxVertices, sizeof(skyboxVertices));
	vao->addAttribute(0, 3, 3 * sizeof(float), 0);
	text = new Texture(clouds2);
	shader = new Shader(vertex, frag);
	shader->setInt(0, "skybox");
}


Skybox::~Skybox() {
	delete text;
	delete shader;
	delete vao;
}

void Skybox::draw() {
	//cout << "draw!\n";
	vao->use();
	shader->use();
	mat4 view = mat4(mat3(cam->view));
	shader->setmat4(view, "view");
	shader->setmat4(cam->projection, "projection");
	text->use(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);
	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}