#version 450

layout ( location = 0 ) in vec2 aVertexPosition;
layout ( location = 1 ) in vec4 aVertexColor;
out vec4 fragVertexColor;

uniform vec4 uTranslationX;

void main() 
{
	gl_Position = vec4 (aVertexPosition, 0.f, 1.f) + uTranslationX;
	fragVertexColor = aVertexColor;
}
