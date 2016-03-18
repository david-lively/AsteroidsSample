#version 150

#define MAX_LIGHTS 16

uniform int LightCount = 0;

uniform vec4 LightColorIntensity[MAX_LIGHTS];
uniform vec3 LightDirection[MAX_LIGHTS];

in vec3 WorldPosition;
in vec4 Color;
out vec4 fragmentColor;

vec3 ProcessLights(vec3 normal)
{
	vec4 color;

	for (int i = 0; i < LightCount; ++i)
	{
		vec4 colorIntensity = LightColorIntensity[i];
		vec3 direction = LightDirection[i];

		float intensity = dot(normal, direction);
		color.rgb += intensity * colorIntensity.rgb;
	}

	color.a = 1;


	return color;
	
}


void main() {
    
    vec3 dx = normalize(dFdx(WorldPosition));
    vec3 dy = normalize(dFdy(WorldPosition));

	vec3 normal = cross(dx,dy);
	vec3 color = ProcessLights(normal);
	
    fragmentColor = vec4(color,1);
}
