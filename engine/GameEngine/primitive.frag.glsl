#version 430 core

uniform float ForceWireframe = 0;

out vec4 fragmentColor;

void main() 
{
	fragmentColor = vec4(1, 1, 1, 1);

    //fragmentColor = vertexColor * clamp(1 - ForceWireframe, 0, 1) + vec4(ForceWireframe);
}
