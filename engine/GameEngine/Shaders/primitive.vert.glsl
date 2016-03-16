#version 150

#define PI (3.141592f)
#define TO_RADIANS(degrees) (degrees * PI / 180.f)


uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;

/// uniforms - same value for all vertices
uniform float GameTimeTotalSeconds;
uniform float TimeScale = 1;

uniform vec4 Color = vec4(0.2f, 0.2f, 0.2f, 1);

in vec3 Pos;

out vec4 vertexColor;

void main()
{
    vec4 position = vec4(Pos,1);
    
    position = Projection * View * World * position;    

	vertexColor = Color;

    gl_Position = position;
}
