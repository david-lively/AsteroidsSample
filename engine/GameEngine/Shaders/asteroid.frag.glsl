#version 330 core

#define PI (3.141592f)
#define TO_RADIANS(x) (x * PI / 180)

#define MAX_LIGHTS 16

uniform mat4 View;
uniform int LightCount = 0;

uniform vec3 LightDirection[MAX_LIGHTS];
uniform vec4 LightColorIntensity[MAX_LIGHTS];
uniform mat4 LightTransform[MAX_LIGHTS];
uniform vec4 EmissiveColorIntensity;

uniform int BreakPlaneCount;
uniform vec3 BreakPlanes[8];

uniform float ForceWireframe = 0;
uniform float ColorByDepth = 0;
uniform float ExplosionFactor = 0.f;

uniform vec3 ExplosionColorInner;
uniform vec3 ExplosionColorOuter;
uniform float ExplosionColorIntensity;

in gOutputType
{
	in vec4 ObjectPosition;
	in vec4 WorldPosition;
	in vec4 Color;
} gOut;


out vec4 fragmentColor;

float saturate(float val)
{
	return max(min(val, 1), 0);
}

vec3 saturate(vec3 val)
{
	return vec3(saturate(val.x), saturate(val.y), saturate(val.z));
}

vec4 saturate(vec4 val)
{
	return vec4(saturate(val.xyz), saturate(val.w));
}

vec3 CalculateLight(vec3 color, float emissive, vec3 pos, vec3 dir, vec3 normal)
{
	vec4 p = View * vec4(pos,1);
	float intensity = saturate(dot(normal, dir));
	return (intensity + emissive) * color;
}

vec3 ProcessLights(vec3 normal)
{
	vec4 color;

	for (int i = 0; i < LightCount; ++i)
	{
		vec4 position = vec4(0, 0, 0, 1);
		position = View * LightTransform[i] * position;

		vec4 colorIntensity = LightColorIntensity[i];
		vec3 direction = LightDirection[i];

		vec3 rgb = CalculateLight(
			LightColorIntensity[i].rgb
			, LightColorIntensity[i].a
			, position.xyz
			, direction
			, normal
			);

		color.rgb += rgb;
	}


	color.a = 1;

	return color;
}

/// calculate brightness (correct grayscale)
float Luminosity(vec3 color)
{
	return 0.2126f * color.r + 0.7152f * color.g + 0.0722 * color.b;
}


void main() {

	vec3 fragmentPosition = gOut.WorldPosition.xyz;

    vec3 dx = normalize(dFdx(fragmentPosition));
    vec3 dy = normalize(dFdy(fragmentPosition));

	vec3 normal = cross(dx,dy);

	vec3 color = EmissiveColorIntensity.rgb * EmissiveColorIntensity.a + ProcessLights(normal);
	
	color *= dot(vec3(0, 0, 1), normal);
	if (abs(dot(vec3(0, 0, -1), normal)) > cos(TO_RADIANS(80)))
		color *= vec3(0.2f);

	//if (ColorByDepth > 0.5f)
	//{
	//	float brightness = 2 * (length(gOut.ObjectPosition)  - 1);
	//	color *= brightness;
	//}

	if (ForceWireframe > 0.5f)
		color = vec3(1);

	float explosionColor = 3 * saturate(1 - ExplosionFactor);
	
	fragmentColor = vec4(color * explosionColor, explosionColor);
}
