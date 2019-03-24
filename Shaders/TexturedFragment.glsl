#version 150

uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex1;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour;

void main(void){
	fragColour = texture(diffuseTex, IN.texCoord);
	
	//fragColour = mix(texture(diffuseTex, IN.texCoord) * IN.colour, texture(diffuseTex1, IN.texCoord), 0.5);
}