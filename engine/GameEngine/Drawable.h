#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Common.h"
#include "WorldEntity.h"
#include "Material.h"
#include "Mesh.h"
#include "UniformCollection.h"

class Drawable :
	public WorldEntity
{
public:
	class Mesh& Mesh;
	class Material& Material;
	UniformCollection Uniforms;

	Drawable(const std::string& name) : WorldEntity(name)
		, Mesh(Create<class Mesh>(name + ".mesh"))
		, Material(Create<class Material>(name + ".material")
		)
	{
		Mesh.Material = &Material;
		Uniforms.Drawable = this;
	}

	Drawable() : Drawable("drawable")
	{

	}

	bool OnInitialize() override;
	void OnRender(const GameTime& time) override;

private:


};

#endif
