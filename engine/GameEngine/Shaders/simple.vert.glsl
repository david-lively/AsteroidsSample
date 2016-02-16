#version 150

#define PI (3.141592f)
#define TO_RADIANS(degrees) (degrees * PI / 180.f)

uniform mat4 World;
uniform mat4 Projection;
uniform mat4 Rotation;


/// uniforms - same value for all vertices
uniform float GameTimeTotalSeconds;
uniform float TimeScale = 1;
uniform vec2 WindowSize;


in vec3 Pos;

out vec4 Color;
out vec4 DevicePosition;


void main()
{
    vec4 position = vec4(Pos,1);
    
    
    float theta = fract(GameTimeTotalSeconds * TimeScale) * 360;

    position = Projection * World * position;
    
    Color = vec4(vec3(position.z > 0),1);
    DevicePosition = position;
    
    gl_Position = position;
}
