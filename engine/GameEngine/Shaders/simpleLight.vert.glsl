#version 150

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
out vec4 WorldPosition;


void main()
{
    vec4 position = vec4(Pos,1);
	WorldPosition = World * position;
	gl_Position = Projection * View * WorldPosition;
    
    Color = vec4(Pos,1);
}
