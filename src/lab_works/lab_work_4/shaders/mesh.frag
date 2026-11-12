#version 450

#define PHONG dot(L,reflect(-L, N))
#define BLINN dot(N, H)

layout( location = 0 ) out vec4 fragColor;

in vec3 vNormal;
in vec3 vPosition;

uniform vec3 uAmbientColor;
uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform float uShininess;
uniform vec3 uLightPosition;

void main()
{
	vec3 N = normalize(vNormal);
	vec3 L = normalize(uLightPosition - vPosition);
	vec3 H = normalize(L - vPosition);

	float diff = dot(N, L);
	if (diff < 0){
		diff = -diff;
	}
	float cosPhi = BLINN;
	if (cosPhi < 0){
		cosPhi = -cosPhi;
	}
	float spec = pow(cosPhi, uShininess);

	fragColor = vec4(uAmbientColor + uDiffuseColor * diff + uSpecularColor * spec, 1.0f);
}
