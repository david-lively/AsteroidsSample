#version 410 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

#include "common.glsl"


uniform int NoiseArrayLength = 128;
uniform float[128] NoiseValues;

uniform float ExplosionFactor = 0.f;
uniform float ExplosionSpeed = 1.f;
uniform float TestFloat = 0.f;

in vec4 Color[3];
in vec4 ObjectPosition[3];

in gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
} gl_in[];


out gOutputType gOut;
//{
//	out vec4 ObjectPosition;
//	out vec4 WorldPosition;
//	out vec4 Color;
//	out vec3 BaryPosition;
//} gOut;

void EmitTriangle(vec3 v0, vec3 v1, vec3 v2)
{
	// float a[5] = float[](3.4, 4.2, 5.0, 5.2, 1.1);
	vec3 verts[] = vec3[]( v0, v1, v2 );

	for (int i = 0; i < 3; ++i)
	{
		vec4 p = vec4(verts[i], 1);

		gl_Position = Projection * View * World * p;
		gOut.ObjectPosition = p;
		gOut.WorldPosition = World * p;
		gOut.Color = vec4(1, 0, 1, 1);
		gOut.BaryPosition = vec3(i == 0, i == 1, i == 2);
		//gOut.BaryPosition.rgb = vec3(1.0f);

		
		
		EmitVertex();
	}

	EndPrimitive();
}

void main()
{
	vec3 v0 = ObjectPosition[0].xyz;
	vec3 v1 = ObjectPosition[1].xyz;
	vec3 v2 = ObjectPosition[2].xyz;

	float sideLength = length(v1 - v0) * 5;

	float noiseValue = NoiseValues[gl_PrimitiveID % NoiseArrayLength]; 

	vec3 center = (v0 + v1 + v2) / 3.f;

	vec3 normal = normalize(cross(v0 - v1, v2 - v1));

	vec3 vtop = (center - normal * sideLength * 2 * (1 + noiseValue));
	
	vec3 v[] = vec3[]( v0, v1, v2, vtop );

	for (int i = 0; i < 4; ++i)
	{
		v[i] += normal * 2 * ExplosionFactor;
	}
	
	EmitTriangle(v[0], v[1], v[2]);

	if (ExplosionFactor > 0)
	{
		// convert face to pyramid for "solid" explosions
		EmitTriangle(v[0], v[1], v[3]);
		EmitTriangle(v[1], v[2], v[3]);
		EmitTriangle(v[2], v[0], v[3]);
	}

}
