#version 330 core
/// Text renderer - see http://github.prideout.net/strings-inside-vertex-buffers/
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;
uniform float GameTimeTotalSeconds = 0.f;

uniform float ExplosionSpeed = 10.f;
uniform float ExplosionFactor = 0.f;

uniform int NoiseArrayLength = 128;
uniform float[128] NoiseValues;


in vec4 Color[3];
in vec4 ObjectPosition[3];

in gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
} gl_in[];

out gOutputType
{
	out vec4 ObjectPosition;
	out vec4 WorldPosition;
	out vec4 Color;
} gOut;

vec2 rotate(vec2 p, float theta)
{
	vec2 result = p;

	float x1 = p.x * cos(theta) - p.y * sin(theta);
	float y1 = p.x * sin(theta) + p.y * cos(theta);

	result.x = x1;
	result.y = y1;

	return result;
}

vec4 rotateZ(vec4 p, float theta)
{
	vec4 result = p;

	float x1 = p.x * cos(theta) - p.y * sin(theta);
	float y1 = p.x * sin(theta) + p.y * cos(theta);

	result.x = x1;
	result.y = y1;

	return result;
}


void main()
{
	vec3 v0 = ObjectPosition[0].xyz;
	vec3 v1 = ObjectPosition[1].xyz;
	vec3 v2 = ObjectPosition[2].xyz;

	vec3 center = (v0 + v1 + v2) / 3.f;

	vec3 normal = normalize(cross(v0 - v1, v2 - v1));
	
	vec3 explodeDirection = normal;

	int noiseIndex = (gl_PrimitiveID * 3 * 100) % NoiseArrayLength;



	for (int i = 0; i < 3; ++i)
	{
		vec4 p = ObjectPosition[i];

		/// nudge towards center of the primitive
		//vec3 toCenter = normalize(center - p.xyz);
		//p.xyz += toCenter * 0.1f;

		p.xyz -= center;
		
		float noise = NoiseValues[noiseIndex];

		p.xy = rotate(p.xy, (GameTimeTotalSeconds + noise * 5) * 3.14159/180 * ExplosionFactor * ExplosionSpeed);

		p.xyz += center;

		p.xyz += explodeDirection * ExplosionFactor * 3;

		gl_Position = Projection * View * World * p;

		gOut.ObjectPosition = ObjectPosition[i];
		gOut.WorldPosition = World * p;
		gOut.Color = vec4(normal, 1);

		EmitVertex();
	}

	EndPrimitive();
}