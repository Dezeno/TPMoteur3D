#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

out vec3 vNormal;
out vec3 vPosition;

uniform mat4 uMVMatrix;
uniform mat4 uMVPMatrix;
uniform mat4 uNormalMatrix;

void main()
{
	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );
	vPosition = (uMVMatrix * vec4( aVertexPosition, 1.f )).xyz;
	vNormal = mat3(uNormalMatrix) * aVertexNormal;
}
