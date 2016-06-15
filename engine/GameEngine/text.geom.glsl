#version 430 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;
uniform float GameTimeTotalSeconds;
uniform float TestFloat = 1;
uniform vec2 WindowSize;
uniform vec2 TextureSize;
uniform float AspectRatio;
uniform float FontSize = 100.f;
/// x,y = position, z = override wvp
uniform vec3 RenderPosition;
uniform vec2 RenderSize = vec2(32, 16);// 1 / 32.f, 1 / 16.f);
uniform int FirstCharacterInTexture = 32; // space

/// number of character cells in the font on each axis
uniform ivec2 FontLayout = ivec2(16, 8);
uniform sampler2D FontTexture;
//uniform vec2 RenderPosition = vec2(0, 0);

in int vCharacter[];
in int vPosition[];

out vec2 gTexCoord;


void main(void)
{
	vec2 pos;

	if (RenderPosition.z > 0)
	{
		mat4 wvp = Projection * View * World;
		vec4 p = wvp * vec4(RenderPosition.xy, 0, 1);
		p /= p.w;
		pos = p.xy;
	}
	else
	{
		pos = RenderPosition.xy;
	}

	vec2 renderSize = vec2(RenderSize.x / WindowSize.x, RenderSize.y / WindowSize.y) * FontSize;

	//vec2 pos = RenderPosition;

	vec2 texelSize = vec2(1.f / TextureSize.x, 1.f / TextureSize.y);
	vec2 glyphSize = 32 * texelSize;

	int character = vCharacter[0] - FirstCharacterInTexture;

	ivec2 cell = ivec2(character % FontLayout.x, character / FontLayout.x);

	vec2 texNW = cell * glyphSize + texelSize;
	vec2 texSE = texNW + glyphSize - texelSize;

	vec2 renderNW = pos + vec2(vPosition[0] * renderSize.x / 2, 0);
	vec2 renderSE = renderNW + vec2(renderSize.x / 2, -renderSize.y);

	gl_Position = vec4(renderNW, 0, 1);
	gTexCoord = texNW;
	EmitVertex();

	gl_Position = vec4(renderSE.x, renderNW.y, 0, 1);
	gTexCoord = vec2(texSE.x, texNW.y);
	EmitVertex();

	gl_Position = vec4(renderNW.x, renderSE.y, 0, 1);
	gTexCoord = vec2(texNW.x, texSE.y);
	EmitVertex();

	gl_Position = vec4(renderSE, 0, 1);;
	gTexCoord = texSE;
	EmitVertex();

	EndPrimitive();
}