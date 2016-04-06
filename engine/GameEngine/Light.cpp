#include "Common.h"
#include "Light.h"
#include "Vectors.h"
#include "GeometryProvider.h"

#include <vector>
#include <string>

using namespace std;


bool Light::OnInitialize()
{
	//Material.FillType = PolygonMode::Line;

	//vector<Vector3> vertices;
	//vector<GLushort> indices;

	//GeometryProvider::Circle(vertices, indices, Vector3(0), 0.6f, 12);

	//Bounds = BoundingSphere::FromVectors(vertices);

	//Material.Build("Shaders/primitive");

	//Mesh.Material = &Material;
	//Mesh.Initialize(vertices, indices);
	//Mesh.Type = BeginMode::Lines;
	//

	return true;
}

//
//void Light::OnRender(const GameTime& time)
//{
//	//Uniforms.SetUniform("Color", this->Color);
//	//WorldEntity::OnRender(time);
//}
//
//
