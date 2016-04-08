#include "Drawable.h"
#include "Game.h"


void Drawable::OnRender(const GameTime& time)
{
	Material.Bind();

	
	Game::Instance().Environment().Apply(*this, time);

	//Uniforms.SetUniform("World", Transform.GetMatrix());
	Uniforms.Apply(time);


	WorldEntity::OnRender(time);
}

bool Drawable::OnInitialize()
{
	Uniforms.Initialize(Material.Shader);

	return WorldEntity::OnInitialize();
}
