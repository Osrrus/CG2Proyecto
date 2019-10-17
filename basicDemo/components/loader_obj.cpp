#pragma once
#include "loader.h"

unsigned int loadT(const char *path)
{
	// std::cout << "path :: "<< path << std::endl;
	unsigned int id;
	// Creates the texture on GPU
	glGenTextures(1, &id);
	// Loads the texture
	int textureWidth, textureHeight, numberOfChannels;
	// Flips the texture when loads it because in opengl the texture coordinates are flipped
	stbi_set_flip_vertically_on_load(true);
	// Loads the texture file data
	unsigned char *data = stbi_load(path, &textureWidth, &textureHeight, &numberOfChannels, 0);
	if (data)
	{
		// Gets the texture channel format
		GLenum format;
		GLenum extraFormat;
		switch (numberOfChannels)
		{
		case 1:
			format = GL_RED;
			extraFormat = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			extraFormat = GL_SRGB;
			break;
		case 4:
			format = GL_RGBA;
			extraFormat = GL_SRGB_ALPHA;

			break;
		}

		// Binds the texture
		glBindTexture(GL_TEXTURE_2D, id);
		// Creates the texture
		glTexImage2D(GL_TEXTURE_2D, 0, extraFormat, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
		// Creates the texture mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set the filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "ERROR:: Unable to load texture " << path << std::endl;
		glDeleteTextures(1, &id);
	}
	// We dont need the data texture anymore because is loaded on the GPU
	stbi_image_free(data);

	return id;
}

Obj *loadObj(const char *path)
{

	FILE *file;
	Obj *newObj = new Obj();
	char lineActula[128], modelId[64];

	if (fopen_s(&file, path, "r") != 0)
	{
		printf("Error al cargar el objeto!\n");
		return NULL;
	}


	std::vector<glm::vec3> auxVertices;
	std::vector<glm::vec3> auxNormals;
	std::vector<glm::vec2> auxUvs;

	std::vector<unsigned int> indexVertices;
	std::vector<unsigned int> indexNormals;
	std::vector<unsigned int> indexUvs;

	while (1)
	{
		if (fscanf_s(file, "%s", &lineActula) == EOF)
			break;

		if (strcmp(lineActula, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			auxVertices.push_back(vertex);
		}
		else if (strcmp(lineActula, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			auxUvs.push_back(uv);
		}
		else if (strcmp(lineActula, "vn") == 0)
		{

			glm::vec3 normals;
			fscanf_s(file, "%f %f %f\n", &normals.x, &normals.y, &normals.z);
			auxNormals.push_back(normals);
		}
		else if (strcmp(lineActula, "f") == 0)
		{

			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				printf("Formato no valido\n");
				return NULL;
			}

			indexVertices.push_back(vertexIndex[0] - 1);
			indexVertices.push_back(vertexIndex[1] - 1);
			indexVertices.push_back(vertexIndex[2] - 1);

			indexUvs.push_back(uvIndex[0] - 1);
			indexUvs.push_back(uvIndex[1] - 1);
			indexUvs.push_back(uvIndex[2] - 1);

			indexNormals.push_back(normalIndex[0] - 1);
			indexNormals.push_back(normalIndex[1] - 1);
			indexNormals.push_back(normalIndex[2] - 1);
		}
	}
	
	newObj->setObj(auxVertices,auxUvs,auxNormals,indexVertices,indexUvs,indexNormals);
	
	fclose(file);

	return newObj;
}