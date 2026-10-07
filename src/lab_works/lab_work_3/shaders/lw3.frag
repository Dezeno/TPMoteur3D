#version 450

layout ( location = 0 ) out vec4 fragColor;
in vec3 fragVertexColor;

void main() 
{
	fragColor = vec4(fragVertexColor, 1.0f);
}
