#ifndef GAMEENVIRONMENT_H
#define GAMEENVIRONMENT_H

#include <string>
#include <vector>
#include <map>

#include "GameObject.h"
#include "Log.h"
#include "Light.h"


class GameEnvironment : public GameObject
{
public:
	const int MaxLights = 16;


	GameEnvironment(const std::string& name) : GameObject(name)
	{

	}

	GameEnvironment() : GameEnvironment("gameEnvironment")
	{

	}

	void SetUniform(int locationn, Light& light)
	{
		m_lights[light.Id] = &light;
	}
	
	void Apply(const GameObject& sender, const GameTime& time);
	
	Light& CreateLight()
	{
		auto& light = Create<Light>("light");

		m_lights[light.Id] = &light;

		return light;
	}

private:
	std::map<int, Light*> m_lights;

};



#endif