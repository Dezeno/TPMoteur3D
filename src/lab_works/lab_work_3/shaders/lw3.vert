#version 450

layout ( location = 0 ) in vec3 aVertexPosition;
layout ( location = 1 ) in vec3 aVertexColor;
out vec3 fragVertexColor;

uniform mat4 uMVPMatrix;

void main() 
{
	gl_Position = uMVPMatrix * vec4 (aVertexPosition, 1.0f);
	fragVertexColor = aVertexColor;
}
