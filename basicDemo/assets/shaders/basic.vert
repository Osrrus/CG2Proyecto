#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertex;
// Atributte 1 of the vertex
layout (location = 1) in vec2 uvs;
// Atributte 2 of the vertex
layout (location = 2) in vec3 normals;

// Vertex data out data
out vec3 vColor;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;

void main()
{  
    gl_Position = Projection* View* Model *vec4(vertex, 1.0f);
}