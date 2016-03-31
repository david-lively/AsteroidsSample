#version 330 core
/// Text renderer - see http://github.prideout.net/strings-inside-vertex-buffers/
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;

uniform float MoveBy;

in vec4 Color[3];
in vec4 ObjectPosition[3];
in vec4 WorldPosition[3];

in gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
} gl_in[];

out gOutputType
{
	out vec4 gObjectPosition;
	out vec4 gWorldPosition;
	out vec4 gColor;
} gOut;


void main()
{
	vec3 normal = (ObjectPosition[0].xyz + ObjectPosition[1].xyz + ObjectPosition[2].xyz) / 3.f;

	for (int i = 0; i < 3; ++i)
	{
		vec4 p = ObjectPosition[i];
		p.xyz += normal.xyz * MoveBy;

		gl_Position = Projection * View  * World * p;
	
		gOut.gObjectPosition = p;
		gOut.gWorldPosition = World * p;
		gOut.gColor = vec4(normal,1);// Color[i];

		EmitVertex();
	}

	EndPrimitive();
}