#version 430 core

in vec3 fColor;
in vec2 fTexCoord;

out vec4 color;

layout (binding = 0) uniform sampler2D uTexture0;
layout (binding = 1) uniform sampler2D uTexture1;

uniform float uTextureMix;

void main()
{
	color = mix(texture(uTexture0, fTexCoord), texture(uTexture1, fTexCoord), uTextureMix);
}
