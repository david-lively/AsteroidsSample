#include <string>

using namespace std;


#include "Sprite.h"
#include "ThirdParty/soil/SOIL2.h"

#include "Vectors.h"
#include "GeometryProvider.h"
#include "Game.h"

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}

bool Sprite::OnInitialize()
{
	auto& meshes = Game::Instance().Models;
	string meshName = "Quad";

	Material.Build("Shaders/textured");
	Material.FillType = PolygonMode::Fill;
	Material.DepthTesting = true;

	if (meshes.Exists(meshName))
	{
		Mesh = *meshes.Get(meshName);
		Transform.Translation.Z = 0;
		Mesh.Material = &Material;
	}
	else
	{
		vector<Vector3> vertices;
		vector<GLushort> indices;

		GeometryProvider::Quad(vertices, indices, 1, 1);
		GeometryProvider::FitToUnitCube(vertices);

		Mesh.Bounds = BoundingSphere::FromVectors(vertices);

		Mesh.Material = &Material;
		Mesh.Initialize(vertices, indices);
		Mesh.Type = BeginMode::Triangles;

		meshes.Add(meshName, &Mesh);
	}
	

	return Drawable::OnInitialize();
}

void Sprite::Load(const std::string& filename, bool invertY)
{
	int flags = invertY ? SOIL_FLAG_INVERT_Y : 0;

	check_gl_error();

	TextureId = SOIL_load_OGL_texture(filename.c_str(), 0, 0, flags, &Width, &Height);

	check_gl_error();
}

void Sprite::OnRender(const GameTime& time)
{

	Material.Bind();

	if (TextureId > 0)
	{

		gl::BindTexture  (gl::TEXTURE_2D, TextureId);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);

		check_gl_error();
	}

	
	Uniforms.SetUniform("InScreenSpace", InScreenSpace);
	Uniforms.SetUniform("Layer", (float)Layer);
	Uniforms.SetUniform("MaxLayers", (float)MaxLayers);

	Drawable::OnRender(time);
}
