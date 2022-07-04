#version 430 core

in vec3 fColor;
in vec2 fTexCoord;

out vec4 color;

uniform sampler2D oTexture;

void main()
{
	color = texture(oTexture, fTexCoord);
}
