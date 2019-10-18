#include "Obj.h"

int Obj::integerID = 1;

Obj::Obj()
{
    this->model = glm::mat4(1.0f);
    glGenVertexArrays(1, &this->VAO);
	glGenBuffers(4, this->VBO);
	this->overallArea = 0.0;
}

Obj::~Obj()
{
    glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(4, this->VBO);
}

void Obj::setObj(std::vector<glm::vec3> Vertex, std::vector<glm::vec2> Uvs, std::vector<glm::vec3> Normals,std::vector<unsigned int> indexV,std::vector<unsigned int> indexU,std::vector<unsigned int>indexN){

    for (int i = 0; i < indexV.size(); i+=3)
	{

		this->vertex.push_back(Vertex[indexV[i]]);
		this->vertex.push_back(Vertex[indexV[i+1]]);
		this->vertex.push_back(Vertex[indexV[i+2]]);

		calculateRGBID();

		overallArea += calculateArea(Vertex[indexV[i]],Vertex[indexV[i+1]],Vertex[indexV[i+2]]);

	}

	for (int i = 0; i < indexU.size(); i++)
	{

		this->uvs.push_back(Uvs[indexU[i]]);
	}

	for (int i = 0; i < indexN.size(); i++)
	{

		this->normals.push_back(Normals[indexN[i]]);
	}

    glBindVertexArray(this->VAO);

	// Sets the buffer geometry data
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(glm::vec3), &this->vertex[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);

	// text Coord
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *)0);

	// normals
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);

	//Vertex triangle ID
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, this->idRGB.size() * sizeof(glm::vec3), &this->idRGB[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);

	glBindVertexArray(0);

	texelArea = 0;
}

void Obj::Draw(){

    glBindVertexArray(this->VAO);

	glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());

	glBindVertexArray(0);
}

void Obj::calculateRGBID() {

	float redValue = integerID % 256;

	int greenRemainingValue = (int)integerID / 256;

	float greenValue = greenRemainingValue % 256;

	int blueRemainingValue = (int)greenRemainingValue / 256;

	float blueValue = blueRemainingValue % 256;

	redValue = redValue / 255.0f;
	greenValue = greenValue / 255.0f;
	blueValue = blueValue / 255.0f;

	glm::vec3 idVector = glm::vec3(redValue, greenValue, blueValue);

	this->idRGB.push_back(idVector);
	this->idRGB.push_back(idVector);
	this->idRGB.push_back(idVector);

	integerID += 10;
}

float Obj::calculateArea(glm::vec3 vertex1,glm::vec3 vertex2,glm::vec3 vertex3) {

	glm::vec3 v1ToV2;
	glm::vec3 v1ToV3;
	glm::vec3 normalVector;

	float area;

	v1ToV2.x = vertex2.x - vertex1.x;
	v1ToV2.y = vertex2.y - vertex1.y;
	v1ToV2.z = vertex2.z - vertex1.z;

	v1ToV3.x = vertex3.x - vertex1.x;
	v1ToV3.y = vertex3.y - vertex1.y;
	v1ToV3.z = vertex3.z - vertex1.z;

	normalVector.x = v1ToV2.y * v1ToV3.z - v1ToV2.z * v1ToV3.y;
	normalVector.y = v1ToV2.z * v1ToV3.x - v1ToV2.x * v1ToV3.z;
	normalVector.z = v1ToV2.x * v1ToV3.y - v1ToV2.y * v1ToV3.x;

	area = 0.5 * sqrt(normalVector.x * normalVector.x + normalVector.y * normalVector.y + normalVector.z * normalVector.z);

	return area;
}