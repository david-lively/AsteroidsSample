#version 150

#define MAX_LIGHTS 16

uniform mat4 View;
uniform int LightCount = 0;

uniform vec4 LightColorIntensity[MAX_LIGHTS];
uniform mat4 LightTransform[MAX_LIGHTS];
uniform float ForceWireframe = 0;
uniform vec4 EmissiveColorIntensity;


in vec4 WorldPosition;
in vec4 Color;
out vec4 fragmentColor;

vec3 ProcessLights(vec3 normal)
{
	vec4 color;

	for (int i = 0; i < LightCount; ++i)
	{
		vec4 colorIntensity = LightColorIntensity[i];
		vec4 position = vec4(0, 0, 0, 1);

		position = View * LightTransform[i] * position;

		vec3 direction = normalize(position.xyz - WorldPosition.xyz);

		float intensity = dot(normal, direction);
		color.rgb += intensity * colorIntensity.rgb;
	}

	color.a = 1;

	return color;
}


void main() {
	vec3 fragmentPosition = WorldPosition.xyz;

    vec3 dx = normalize(dFdx(fragmentPosition));
    vec3 dy = normalize(dFdy(fragmentPosition));

	vec3 normal = cross(dx,dy);

	vec3 color = EmissiveColorIntensity.rgb * EmissiveColorIntensity.a + ProcessLights(normal);
	
	float force = clamp(ForceWireframe, 0, 1);

	color.rgb = (1 - force) * color + force * vec3(1);

	fragmentColor = vec4(color,1);
}
