#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <glm/glm.hpp>
#include "helpers.h"

class Obj
{
    private:
    public:
        glm::mat4 model;
        std::vector<glm::vec3>vertex,normals;
        std::vector<glm::vec2>uvs;
        unsigned int VAO;
        unsigned int VBO[3];
        void setObj(std::vector<glm::vec3>, std::vector<glm::vec2>, std::vector<glm::vec3>,std::vector<unsigned int>,std::vector<unsigned int>,std::vector<unsigned int>);
        Obj();
        ~Obj();
        void Draw();
};


