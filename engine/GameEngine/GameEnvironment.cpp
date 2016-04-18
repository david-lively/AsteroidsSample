#include "GameEnvironment.h"

#include <random>
#include <vector>

#include "GameObject.h"
#include "Material.h"
#include "Game.h"
#include "PerlinNoise.h"
#include "Vectors.h"
#include "Matrix.h"
#include "Drawable.h"

using namespace std;

void GameEnvironment::Apply(Drawable& drawable, const GameTime& time)
{
	ApplyLights (drawable, time);
	ApplyGlobals(drawable, time);
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

void GameEnvironment::ApplyLights(Drawable& drawable, const GameTime& time)
{
	auto& uniforms = drawable.Uniforms;

	auto countIndex = uniforms.GetUniformLocation("LightCount");
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

	GLint location = uniforms.GetUniformLocation("LightColorIntensity");
	if (location >= 0)
		gl::Uniform4fv(location, lightCount, (const GLfloat*)lightColorIntensities.data());

	check_gl_error();

	location = uniforms.GetUniformLocation("LightDirection");
	if (location >= 0)
		gl::Uniform3fv(location, lightCount, (const GLfloat*)lightDirections.data());

	check_gl_error();

	location = uniforms.GetUniformLocation("LightPosition");
	if (location >= 0)
		gl::Uniform3fv(location, lightCount, (const GLfloat*)lightPosition.data());

	check_gl_error();

	location = uniforms.GetUniformLocation("LightTransform");
	if (location >= 0)
		gl::UniformMatrix4fv(location, lightCount, false, (GLfloat*)lightTransform.data());
}

void GameEnvironment::OnWindowResize(const int width, const int height)
{
	GameObject::OnWindowResize(width, height);

	WindowWidth = width;
	WindowHeight = height;

	m_windowSize.X = width;
	m_windowSize.Y = height;
}


void GameEnvironment::ApplyGlobals(Drawable& drawable, const GameTime& time)
{
	auto& camera = Game::Camera();
	auto& uniforms = drawable.Uniforms;
	auto location = uniforms.GetUniformLocation("NoiseValues");

	if (location >= 0)
	{
		gl::Uniform1fv(location, m_noiseValues.size(), m_noiseValues.data());
	}

	uniforms.SetUniform("NoiseArrayLength", (int)m_noiseValues.size());
	uniforms.SetUniform("GameTimeTotalSeconds", Game::Instance().Time.TotalSeconds());
	uniforms.SetUniform("ForceWireframe", ForceWireframe);
	uniforms.SetUniform("World", m_matrixStack.top());

	uniforms.SetUniform("WindowSize", m_windowSize);
	uniforms.SetUniform("AspectRatio", m_windowSize.X * 1.f / m_windowSize.Y);

	uniforms.SetUniform("TestFloat", TestFloat);
}

void GameEnvironment::GenerateNoiseValues(std::vector<float>& arr, int count)
{
	PerlinNoise noise;
	srand(time(NULL));

	arr.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		float noiseValue = (rand() % 100) / 100.f;
		arr.push_back(noiseValue);
	}

}
