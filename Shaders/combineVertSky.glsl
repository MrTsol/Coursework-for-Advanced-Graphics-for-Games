#version 150 core

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

in vec3 position;
in vec2 texCoord;

out Vertex
{
	vec2 texCoord;
	vec3 skyNormal;
}
OUT;

void main(void)
{
	vec3 tempPos = position - vec3(0, 0, 1);
	OUT.skyNormal = transpose(mat3(viewMatrix)) * normalize(tempPos);

	gl_Position = projMatrix * vec4(position, 1.0);
	OUT.texCoord = texCoord;
}
