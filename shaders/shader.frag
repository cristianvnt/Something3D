#version 330 core

in vec3 vPosOut;
in vec3 vertexColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(vPosOut, 1.f);
}