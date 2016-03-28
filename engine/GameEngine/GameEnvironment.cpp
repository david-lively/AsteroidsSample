#include "GameObject.h"
#include "GameEnvironment.h"
#include "Material.h"

#include <vector>

using namespace std;

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


	vector<Vector4> lightColorIntensities;
	lightColorIntensities.reserve(m_lights.size());

	vector<Vector3> lightDirections;
	lightDirections.reserve(m_lights.size());

	vector<Vector3> lightPosition;
	lightPosition.reserve(m_lights.size());

	vector<Matrix> lightTransform;
	lightTransform.reserve(m_lights.size());

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

			Matrix lightMatrix;
			light.BuildCombinedMatrix(lightMatrix);
			lightTransform.push_back(lightMatrix);
		}

	}

	GLint lightCount = (GLint)min(lightColorIntensities.size(), lightDirections.size());
	gl::Uniform1i(countIndex, lightCount);

	check_gl_error();

	GLint location = mat->GetUniformLocation("LightColorIntensity");
	if (location >= 0)
		gl::Uniform4fv(location, lightCount, (const GLfloat*)lightColorIntensities.data());

	check_gl_error();

	location = mat->GetUniformLocation("LightDirection");
	if (location >= 0)
		gl::Uniform3fv(location, lightCount, (const GLfloat*)lightDirections.data());

	check_gl_error();

	location = mat->GetUniformLocation("LightPosition");
	if (location >= 0)
		gl::Uniform3fv(location, lightCount, (const GLfloat*)lightPosition.data());

	check_gl_error();
	
	location = mat->GetUniformLocation("LightTransform");
	if (location >= 0)
		gl::UniformMatrix4fv(location, lightCount, false, (GLfloat*)lightTransform.data());

}

void GameEnvironment::PushMatrix(const Matrix& m)
{
	if (m_matrixStack.size() > 0)
		m_matrixStack.push(m * m_matrixStack.top());
	else
		m_matrixStack.push(m);
}

void GameEnvironment::PopMatrix()
{
	m_matrixStack.pop();
}

const Matrix& GameEnvironment::CurrentMatrix()
{

	return m_matrixStack.top();
}


