#include "Obj.h"

Obj::Obj()
{
    this->model = glm::mat4(1.0f);
    glGenVertexArrays(1, &this->VAO);
	glGenBuffers(3, this->VBO);
}

Obj::~Obj()
{
    glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(3, this->VBO);
}

void Obj::setObj(std::vector<glm::vec3> Vertex, std::vector<glm::vec2> Uvs, std::vector<glm::vec3> Normals,std::vector<unsigned int> indexV,std::vector<unsigned int> indexU,std::vector<unsigned int>indexN){

    for (int i = 0; i < indexV.size(); i++)
	{

		this->vertex.push_back(Vertex[indexV[i]]);
	}

	for (int i = 0; i < indexU.size(); i++)
	{

		this->uvs.push_back(Uvs[indexU[i]]);
	}

	for (int i = 0; i < indexN.size(); i++)
	{

		this->normals.push_back(Normals[indexN[i]]);
	}

	this->color.x = rand();
	this->color.y = rand();
	this->color.z = rand();

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

	glBindVertexArray(0);

}

void Obj::Draw(){

    glBindVertexArray(this->VAO);

	glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());

	glBindVertexArray(0);
}