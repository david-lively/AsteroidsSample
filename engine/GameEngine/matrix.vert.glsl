#version 430 core

#define PI (3.141592f)
#define TO_RADIANS(degrees) (degrees * PI / 180.f)

uniform mat4 RotateZ;
uniform mat4 RotateY;
uniform mat4 Projection;


/// uniforms - same value for all vertices
uniform float GameTimeTotalSeconds;
uniform float TimeScale = 1;
uniform vec2 WindowSize;

in vec3 Position;

out vec4 Color;


void main()
{
    vec4 position = vec4(Pos,1);
    
    position = RotateY * position;
    
    
    Color = vec4(position.xyz, 1);
    Color.b = Pos.Z > 0 ? 1 : 0;
    
    gl_Position = position;
}
