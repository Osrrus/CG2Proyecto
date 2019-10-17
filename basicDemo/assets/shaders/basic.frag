#version 330 core
// Fragment Color
out vec4 finalColor;
in vec3 vColor;

void main()
{
    finalColor = vec4(vColor,1.0f);
}