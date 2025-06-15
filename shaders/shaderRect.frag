#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 theColor;

uniform vec4 ourColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float visible;

void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.f - TexCoord.x, TexCoord.y)), visible);
}