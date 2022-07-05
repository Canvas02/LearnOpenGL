#version 430 core

in vec3 fColor;
in vec2 fTexCoord;

out vec4 color;

uniform sampler2D oTexture1;

void main()
{
	color = texture(oTexture1, fTexCoord) * vec4(fColor, 1.0);
}
