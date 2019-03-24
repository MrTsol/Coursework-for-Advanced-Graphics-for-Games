#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D emissiveTex;
uniform sampler2D specularTex;
uniform sampler2D depthTex;
uniform sampler2D skyboxTex;

in Vertex
{
	vec2 texCoord;
	vec3 skyNormal;
}
IN;

out vec4 fragColour;

void main(void)
{
	vec3 diffuse = texture(diffuseTex, IN.texCoord).xyz;
	vec3 light = texture(emissiveTex, IN.texCoord).xyz;
	vec3 specular = texture(specularTex, IN.texCoord).xyz;
	float depth = texture(depthTex, IN.texCoord).x;
	vec3 skybox = texture(skyboxTex, IN.texCoord).xyz;
	
	if(depth == 1.0)
		fragColour = vec4(skybox, 1.0);
	else {
		fragColour.xyz = diffuse * 0.2;
		fragColour.xyz += diffuse * light;
		fragColour.xyz += specular;
		fragColour.a = 1.0;
	}
}