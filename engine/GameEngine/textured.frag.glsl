#version 330 core

uniform sampler2D Texture0;
uniform float TestFloat;
uniform float ForceWireframe = 0;

uniform float GameTimeTotalSeconds;

out vec4 fragmentColor;

in vec2 texCoord;


vec2 rotZ(vec2 v, float theta)
{
	float x1 = v.x * cos(theta) - v.y * sin(theta);
	float y1 = v.x * sin(theta) + v.y * cos(theta);

	return vec2(x1, y1);
}	

void main() 
{
	vec4 texel = texture(Texture0, texCoord);

	fragmentColor = texel;

	if (ForceWireframe > 0)
		fragmentColor = vec4(1);

	//fragmentColor = vec4(texCoord, 0, 1);

    //fragmentColor = vertexColor * clamp(1 - ForceWireframe, 0, 1) + vec4(ForceWireframe);
}
