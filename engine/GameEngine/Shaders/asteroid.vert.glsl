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

void main()
{
	Color = ObjectPosition;
	
	ObjectPosition = vec4(Position, 1);

	if (Broken > 0 && ObjectPosition.x > 0)
		ObjectPosition.x = 0;


	gl_Position = ObjectPosition;
}
