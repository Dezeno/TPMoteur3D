#version 450

layout ( location = 0 ) out vec4 fragColor;
in vec4 fragVertexColor;

uniform float uLuminosite;

void main() 
{
	fragColor = fragVertexColor * uLuminosite;
}
