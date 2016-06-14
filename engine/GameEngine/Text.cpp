#include <vector>
#include <string>

using namespace std;

#include "Text.h"
#include "GeometryProvider.h"
#include "ThirdParty/soil/SOIL2.h"
#include "Game.h"

bool Text::OnInitialize()
{
	vector<char> data;

	string test = "0123";
	data.insert(end(data), begin(test), end(test));

	Material.FillType = PolygonMode::Fill;
	Mesh.Type = BeginMode::Points;
	Material.Build("text");
	//Material.Blend = false;
	Material.DepthTesting = false;

	Mesh.Material = &Material;
	Mesh.IsDynamic = false;
	Mesh.Initialize(data);
	Mesh.CullBackfaces = false;


	auto fontFile = "textures/courier_sdf.png";

	FontTextureId = SOIL_load_OGL_texture(fontFile, 0, 0, 0, &m_fontTextureWidth, &m_fontTextureHeight);

	CellSize = Vector2(32, 32);

	TextureSize = Vector2(m_fontTextureWidth, m_fontTextureHeight);

	return Drawable::OnInitialize();
}

void Text::OnUpdate(const GameTime& time)
{
	Drawable::OnUpdate(time);
}


void Text::OnRender(const GameTime& time)
{

	string text = "FPS: " + to_string((int)time.FramesPerSecond())
		+ " Frame " + to_string(time.FrameNumber())
		;

	Mesh.SetVertexData(Data, BufferUsageHint::DynamicDraw);

	Material.Bind();

	Uniforms.SetUniform("TexelSize", Vector2(1.f / TextureSize.X, 1.f / TextureSize.Y));
	
	Uniforms.SetUniform("TextureSize", TextureSize);
	Uniforms.SetUniform("CellSize", CellSize);
	Uniforms.SetUniform("FirstCharacterInTexture", (int)' ');
	Uniforms.SetUniform("FontSize", FontSize);


	if (FontTextureId > 0)
	{
		auto loc = Uniforms.GetUniformLocation("FontTexture");
		if (loc < 0)
			throw;
		check_gl_error();
		gl::Uniform1i(loc, 0);
		check_gl_error();

		gl::ActiveTexture(gl::TEXTURE0);
		check_gl_error();
		gl::BindTexture(gl::TEXTURE_2D, FontTextureId);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);

		check_gl_error();
	}


	Vector3 uniformRenderPos(RenderPosition.X, RenderPosition.Y, 1);
	Uniforms.SetUniform("RenderPosition", uniformRenderPos);



	Drawable::OnRender(time);
}

