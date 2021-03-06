#ifndef LIGHT_H
#define LIGHT_H

#include "Vectors.h"
#include "WorldEntity.h"

enum class LightType
{
	Point
	,Spot
	,Directional
};

struct LightUniform
{
	Vector4 Color;
	Vector3 Direction;
	float Intensity;
};

class Light : public WorldEntity
{
public:
	Vector3 Position;
	Vector4 Color;
	Vector3 Direction;
	float Specularity;
	float Intensity;


	Light(const std::string& name)
	{

	}


	Light() : Color(1, 1, 1, 1)
	{

	}

	bool OnInitialize();

	//void OnRender(const GameTime& time) override;
};



#endif