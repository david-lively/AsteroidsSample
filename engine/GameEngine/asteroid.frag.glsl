#version 330 core

#include "common.glsl"
#include "lighting.glsl"

out vec4 fragmentColor;

uniform float ExplosionFactor = 0;


void main() {

	vec3 fragmentPosition = gOut.WorldPosition.xyz;

    vec3 dx = normalize(dFdx(fragmentPosition));
    vec3 dy = normalize(dFdy(fragmentPosition));

	vec3 normal = cross(dx,dy);

	vec3 color = light(normal);//EmissiveColorIntensity.rgb * EmissiveColorIntensity.a + ProcessLights(normal).rgb;
	
	color *= dot(vec3(0, 0, 1), normal);
	if (abs(dot(vec3(0, 0, -1), normal)) > cos(TO_RADIANS(80)))
		color *= vec3(0.2f);

	if (ColorByDepth > 0.5f)
	{
		float brightness = 2 * (length(gOut.ObjectPosition)  - 1);
		color *= brightness;
	}

	if (ForceWireframe > 0.5f)
		color = vec3(1);

	float explosionColor = 3 * saturate(1 - ExplosionFactor);
	
	fragmentColor = vec4(color * explosionColor, explosionColor);
}
