#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL

#include "common.glsl"

#define MAX_LIGHTS 16

uniform int LightCount = 0;

uniform vec3 LightDirection[MAX_LIGHTS];
uniform vec4 LightColorIntensity[MAX_LIGHTS];
uniform mat4 LightTransform[MAX_LIGHTS];
uniform vec4 EmissiveColorIntensity;
uniform float ColorByDepth = 0;

in vec4 ObjectPosition;
in vec4 WorldPosition;
in vec4 Color;

in gOutputType
{
	in vec4 ObjectPosition;
	in vec4 WorldPosition;
	in vec4 Color;
} gOut;



vec4 ProcessLights(vec3 normal)
{
	vec4 color;

	for (int i = 0; i < LightCount; ++i)
	{
		vec4 colorIntensity = LightColorIntensity[i];
		vec4 position = vec4(0, 0, 0, 1);

		position = View * LightTransform[i] * position;

		vec3 direction = LightDirection[i];

		float intensity = saturate(dot(normal, direction));
		color.rgb += intensity * colorIntensity.rgb;
	}

	color.a = 1;

	return color;
}

/// calculate brightness (correct grayscale)
float Luminosity(vec3 color)
{
	return 0.2126f * color.r + 0.7152f * color.g + 0.0722 * color.b;
}

vec3 light(vec3 normal)
{
	return EmissiveColorIntensity.rgb * EmissiveColorIntensity.a + ProcessLights(normal).rgb;
}

#endif