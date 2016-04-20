#version 330 core

#define PI (3.141592f)
#define TO_RADIANS(degrees) (degrees * PI / 180.f)

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;
uniform float Broken;

/// uniforms - same value for all vertices
uniform float GameTimeTotalSeconds;
uniform float TimeScale = 1;

in vec3 Position;

out vec4 Color;
out vec4 ObjectPosition;

uniform int BreakPlaneCount;
uniform vec3 BreakPlanes[8];


vec3 BreakVertex(vec3 v)
{
	// projected_point = point - dist*normal;
	for (int i = 0; i < BreakPlaneCount; ++i)
	{
		vec3 n = BreakPlanes[i];

		float d = dot(n, v);

		if (d < 0)
			v = v - d * n;
	}

	return v;
}

void main()
{
	Color = ObjectPosition;
	
	ObjectPosition = vec4(BreakVertex(Position), 1);

	gl_Position = ObjectPosition;
}
