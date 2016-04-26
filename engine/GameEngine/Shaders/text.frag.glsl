#version 330 core

uniform sampler2D FontTexture;

uniform float ForceWireframe;
uniform float TestFloat;
uniform float SignedDistanceThreshold = 0.4;

in vec2 gTexCoord;

out vec4 fragmentColor;

void main() 
{
	if (ForceWireframe > 0)
	{
		fragmentColor = vec4(1);
		return;
	}


	vec2 tex = gTexCoord;




	float d = texture(FontTexture, tex).r;

	if (d > SignedDistanceThreshold)
		fragmentColor = vec4(1);
	else
		discard;

}