#version 410 core

#define PI (3.141592f)
#define TO_RADIANS(x) (x * PI / 180)

#define MAX_LIGHTS 16

uniform mat4 View;
uniform int LightCount = 0;

uniform vec3 LightDirection[MAX_LIGHTS];
uniform vec4 LightColorIntensity[MAX_LIGHTS];
uniform mat4 LightTransform[MAX_LIGHTS];
uniform vec4 EmissiveColorIntensity;

uniform float ForceWireframe = 0;
uniform float ColorByDepth = 0;


in gOutputType
{
	in vec4 ObjectPosition;
	in vec4 WorldPosition;
	in vec4 Color;
} gOut;


//in vec4 ObjectPosition;
//in vec4 WorldPosition;
//in vec4 Color;

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

vec3 ProcessLights(vec3 normal)
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


void main() {

	vec3 fragmentPosition = gOut.WorldPosition.xyz;

    vec3 dx = normalize(dFdx(fragmentPosition));
    vec3 dy = normalize(dFdy(fragmentPosition));

	vec3 normal = cross(dx,dy);

	vec3 color = EmissiveColorIntensity.rgb * EmissiveColorIntensity.a + ProcessLights(normal);
	
	if (abs(dot(vec3(0, 0, 1), normal)) > cos(TO_RADIANS(80)))
		discard;
	else
		color = vec3(1);


	//if (ColorByDepth > 0.5f)
	//{
	//	float c = length(gOut.ObjectPosition) / 5.f;
	//	color = vec3(c);
	//}

	if (ForceWireframe > 0.5f)
		color = vec3(1);

	fragmentColor = vec4(color,1);
}
