#include "Light.h"
#include "Common.h"
#include "Vectors.h"
#include "Matrix.h"
#include "Material.h"
#include "Mesh.h"
#include "GeometryProvider.h"

#include <vector>
#include <string>

using namespace std;


bool Light::OnInitialize()
{
	auto& material = Create<class Material>("ship-material");
	m_material = &material;

	material.FillType = PolygonMode::Line;

	auto& mesh = Create<Mesh>("light-mesh");

	vector<Vector3> vertices;
	vector<GLushort> indices;

	GeometryProvider::Circle(vertices, indices, Vector3(0), 0.6f, 12);

	Bounds = BoundingBox::FromVectors(vertices);

	material.Build("Shaders/primitive");

	mesh.Material = &material;

	mesh.Initialize(vertices, indices);
	mesh.Type = BeginMode::Lines;
	
	return true;
}


void Light::OnRender(const GameTime& time)
{
	WorldEntity::OnRender(time);
	m_material->SetUniform("Color", this->Color);
}


