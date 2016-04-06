#include "Drawable.h"
#include "Game.h"


void Drawable::OnRender(const GameTime& time)
{
	Material.Bind();

	//Uniforms.SetUniform("World", Transform.GetMatrix());
	Uniforms.Apply(time);
	
	Game::Instance().Environment().Apply(*this, time);

	WorldEntity::OnRender(time);
}

bool Drawable::OnInitialize()
{
	Uniforms.Initialize(Material.Shader);

	return WorldEntity::OnInitialize();
}
