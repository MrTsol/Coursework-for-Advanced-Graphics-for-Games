#version 150 core

uniform samplerCube cubeTex;
uniform vec3 cameraPos;

in Vertex
{
	vec3 normal;
}
IN;

out vec4 fragColour[2];

void main(void)
{
	fragColour[0] = texture(cubeTex,normalize(IN.normal));
	//if(depthCol.rgb == vec3(0,0,0))
		//fragColour = texture(cubeTex, normalize(IN.normal));

	//fragColour = depthCol;
}