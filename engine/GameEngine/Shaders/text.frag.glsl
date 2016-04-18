#version 330 core

uniform sampler2D FontTexture;

uniform float ForceWireframe;
uniform float TestFloat;
uniform float SignedDistanceThreshold = 0.4;

in vec2 gTexCoord;

out vec4 fragmentColor;

void main() 
{
	float d = texture(FontTexture, gTexCoord).r;

	if (d > SignedDistanceThreshold)
		fragmentColor = vec4(1);
	else
		discard;

}