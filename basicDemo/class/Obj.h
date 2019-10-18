#pragma once

#include "../define.h"
#include "helpers.h"

class Obj
{
    private:
    public:

        glm::mat4 model;

        std::vector<glm::vec3>vertex,normals,idRGB;
        std::vector<glm::vec2>uvs;
        std::vector<float>area;
        float overallArea;
        float texelArea;

		glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO[4];
        static int integerID;

        void setObj(std::vector<glm::vec3>, std::vector<glm::vec2>, std::vector<glm::vec3>,std::vector<unsigned int>,std::vector<unsigned int>,std::vector<unsigned int>);
        Obj();
        ~Obj();
        void Draw();
        void calculateRGBID();
        float calculateArea(glm::vec3,glm::vec3,glm::vec3);
};


