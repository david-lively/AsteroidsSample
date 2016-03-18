#include "GameObject.h"
#include "GameEnvironment.h"
#include "Material.h"

void GameEnvironment::Apply(const GameObject& sender, const GameTime& time)
{
	const Material* mat = dynamic_cast<const Material*>(&sender);

	if (nullptr == mat)
	{
		mat = sender.GetFirst<Material>();
	}

	if (nullptr == mat)
	{
		Log::Error << "GameEnvironment::Apply called, but could not find a material!\n";
		return;
	}

	auto countIndex = mat->GetUniformLocation("LightCount");
	if (countIndex < 0)
		return;


	std::vector<Vector4> lightColorIntensities;
	lightColorIntensities.reserve(m_lights.size());

	std::vector<Vector3> lightDirections;
	lightDirections.reserve(m_lights.size());

	for (auto lightPtr : m_lights)
	{
		auto light = *lightPtr.second;
		if (light.Enabled)
		{
			Vector4 colorIntensity;

			colorIntensity.X = light.Color.X;
			colorIntensity.Y = light.Color.Y;
			colorIntensity.Z = light.Color.Z;
			colorIntensity.W = light.Intensity;

			lightColorIntensities.push_back(colorIntensity);
			lightDirections.push_back(light.Direction);
		}

	}

	auto lightCount = min(lightColorIntensities.size(), lightDirections.size());
	gl::Uniform1i(countIndex, lightCount);

	auto location = mat->GetUniformLocation("LightColorIntensity");
	if (location >= 0)
		gl::Uniform4fv(location, lightCount, (const GLfloat*)lightColorIntensities.data());

	location = mat->GetUniformLocation("LightDirection");
	if (location >= 0)
		gl::Uniform3fv(location, lightCount, (const GLfloat*)lightDirections.data());
}
