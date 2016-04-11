#include <string>

using namespace std;


#include "Sprite.h"
#include "ThirdParty/soil/SOIL2.h"

#include "Vectors.h"
#include "GeometryProvider.h"


Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}

bool Sprite::OnInitialize()
{
	vector<Vector3> vertices;
	vector<GLushort> indices;

	Material.FillType = PolygonMode::Fill;
	Mesh.Type = BeginMode::Triangles;
	Material.Build("Shaders/textured");
	//Material.Blend = false;
	Material.DepthTesting = false;

	GeometryProvider::Quad(vertices, indices, 1, 1);

	GeometryProvider::FitToUnitCube(vertices);
	Bounds = BoundingSphere::FromVectors(vertices);


	Mesh.Material = &Material;
	Mesh.Initialize(vertices, indices);

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
	
	Drawable::OnRender(time);
}
