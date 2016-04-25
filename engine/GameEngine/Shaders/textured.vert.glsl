#version 330 core

#define PI (3.141592f)
#define TO_RADIANS(degrees) (degrees * PI / 180.f)


uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;
uniform float InScreenSpace;
uniform float TestFloat;

/// uniforms - same value for all vertices
uniform float GameTimeTotalSeconds;
uniform float TimeScale = 1;

uniform float Layer;
uniform float MaxLayers;


uniform vec4 Color = vec4(0.2f, 0.2f, 0.2f, 1);

in vec3 Position;

out vec4 vertexColor;
out vec2 texCoord;


void main()
{
	vec4 position = vec4(Position, 1);
	
	texCoord = position.xy;
	texCoord.x += 0.5f;
	texCoord.y = (texCoord.y - 0.5f) * -1;


	if (InScreenSpace <= 0)
	{
		position.xy *= 20;
		position = Projection * View * World * position;
	}
	else
	{
		position = World * position;
		position.z = clamp(2 * Layer / MaxLayers - 1f, -1, +1);
		position = Projection * position;
		//position.z = TestFloat;
		//position = position;
	}

	vertexColor = Color;

    gl_Position = position;
}
