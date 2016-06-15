#version 430 core
/// http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/

in int Character;

out int vCharacter;
out int vPosition;

void main()
{
	vCharacter = Character;
	vPosition = gl_VertexID;
	gl_Position = vec4(0,0,0,1);
}

