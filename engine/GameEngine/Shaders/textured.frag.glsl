#version 330 core

uniform sampler2D Texture0;
uniform float ForceWireframe = 0;

out vec4 fragmentColor;

in vec2 texCoord;

void main() 
{
	vec4 texel = texture(Texture0, texCoord);
	texel.a = texel.r;
	fragmentColor = texel;

	//fragmentColor = vec4(texCoord, 0, 1);

    //fragmentColor = vertexColor * clamp(1 - ForceWireframe, 0, 1) + vec4(ForceWireframe);
}
