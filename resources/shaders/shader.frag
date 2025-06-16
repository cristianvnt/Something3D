#version 330 core
out vec4 FragColor;

in vec3 vPosOut;

void main()
{
	FragColor = vec4(vPosOut, 1.f);
}