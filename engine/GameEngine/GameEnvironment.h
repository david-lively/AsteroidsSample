#ifndef GAMEENVIRONMENT_H
#define GAMEENVIRONMENT_H

#include <string>
#include <vector>
#include <map>
#include <stack>

#include "GameObject.h"
#include "Log.h"
#include "Light.h"
#include "Matrix.h"


class GameEnvironment : public GameObject
{
public:
	const int MaxLights = 16;

	GameEnvironment(const std::string& name) : GameObject(name)
	{
		GenerateNoiseValues(m_noiseValues, 128);
	}

	GameEnvironment() : GameEnvironment("gameEnvironment")
	{

	}

	float Contrast = 1.f;

	// top-most world matrix from object hierarchy
	const Matrix& CurrentMatrix();
	// add a new matrix to the stack. value is multiplied by top value if available.
	void PushMatrix(const Matrix& m);

	// remove top element from stack.
	void PopMatrix();

	size_t MatrixStackSize() { return m_matrixStack.size(); }

	void SetUniform(int location, Light& light)
	{
		m_lights[light.Id] = &light;
	}
	
	void Apply(Material& material, const GameTime& time);
	
	Light& CreateLight()
	{
		auto& light = Create<Light>("light");

		m_lights[light.Id] = &light;

		return light;
	}

	bool ForceWireframe = false;

private:
	std::map<int, Light*> m_lights;
	std::stack<Matrix> m_matrixStack;
	std::vector<float> m_noiseValues;

	void ApplyLights(Material& material, const GameTime& time);
	void ApplyGlobals(Material& material, const GameTime& time);
	void GenerateNoiseValues(std::vector<float>& arr, int count);

};



#endif