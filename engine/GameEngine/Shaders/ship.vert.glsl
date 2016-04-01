#version 330 core

#define PI (3.141592f)
#define TO_RADIANS(degrees) (degrees * PI / 180.f)

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;

/// uniforms - same value for all vertices
uniform float GameTimeTotalSeconds;
uniform float TimeScale = 1;

in vec3 Position;

out vec4 Color;
out vec4 ObjectPosition;

void main()
{
	ObjectPosition = vec4(Position, 1);
	Color = ObjectPosition;

	gl_Position = ObjectPosition;
}
