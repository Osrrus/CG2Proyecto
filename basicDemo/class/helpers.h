#pragma once
#include "../define.h"

struct MaterialColor {

		glm::vec3 aMaterial;
		glm::vec3 dMaterial;
		glm::vec3 sMaterial;
		float nsMaterial;
		float rMaterial;
		float refractive;
		bool refra;
};

struct Texture
{
	unsigned int id;
	std::string name;
};

struct DrawParameters
{
	glm::mat4 view;
	glm::vec3 viewPos;
	glm::mat4 projection;
	unsigned int skyboxTexture;
	unsigned int deepTexture;
	unsigned int depthMapFB;
	unsigned int depthTexture;
	glm::mat4 lightMatrix;
	
};

