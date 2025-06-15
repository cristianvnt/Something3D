#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vPosOut;
out vec2 TexCoord;
out vec3 theColor;

uniform float offset;

void main()
{
	gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.f);
	vPosOut = aPos;
	TexCoord = aTexCoord;
	theColor = aColor;
}