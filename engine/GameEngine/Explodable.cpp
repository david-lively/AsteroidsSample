//  Explodable.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include <vector>
#include <cmath>

#include "Common.h"
#include "Explodable.h"

using namespace std;

void Explodable::OnRender(const GameTime& time)
{
	Material.Bind();

	Uniforms.SetUniform("ExplosionFactor", ExplosionFactor);
	Uniforms.SetUniform("IsExploding", IsExploding ? 1.f : 0.f);
	Uniforms.SetUniform("ExplosionSpeed", ExplosionSpeed);

	int breakPlaneCount = BreakPlanes.size();

	Uniforms.SetUniform("Broken", Broken);
	Uniforms.SetUniform("BreakPlanes", BreakPlanes);
	Uniforms.SetUniform("BreakPlaneCount", (int)BreakPlanes.size());

	Drawable::OnRender(time);
}


void Explodable::Explode(const GameTime& time, const float duration)
{
	ExplosionTime = 0.f;

	if (duration > 0)
		ExplosionDuration = duration;

	IsExploding = true;
	ExplosionSpeed = 10.f;
}

void Explodable::OnReset(const GameTime& time)
{
	ExplosionFactor = 0.f;
	ExplosionTime = 0.f;
	ExplosionDuration = 0.f;
	IsExploding = false;

	Broken = false;
	BreakPlanes.clear();
}



void Explodable::Break(const GameTime& time, const Vector3& impactPoint, bool pushBackward)
{
	Log::Info << "Break " << Name << endl;

	auto dir = (impactPoint - Transform.Translation).Normalized();
	dir.Z = 0;
	dir.Normalize();

	// rotate normal 90 degrees to get push direction
	// Y = X, X =-1

	std::swap(dir.X, dir.Y);

	dir.X *= -1.f;

	if (pushBackward)
		dir *= -1.f;

	auto velocity = Transform.Velocity();

	Transform.Push(dir * 0.01f + velocity * 0.5f);

	BreakPlanes.push_back(dir);
}

void Explodable::OnPreUpdate(const GameTime& time)
{
	bool wasExploding = IsExploding;

	if (IsExploding)
	{
		ExplosionTime += time.ElapsedSeconds();

		if (ExplosionTime >= ExplosionDuration)
		{
			ExplosionTime = 0.f;
			IsExploding = false;
			ExplosionFactor = 0.f;

			if (nullptr != OnExploded)
				OnExploded(time, *this);

		}
		else
		{
			ExplosionSpeed /= 2.f;
			ExplosionFactor = lerp(0.f, 1.f, ExplosionTime / ExplosionDuration);
		}

	}
	else
		ExplosionTime = 0.f;

}

void Explodable::InvertBreakPlanes()
{
	for (auto it = begin(BreakPlanes); it != end(BreakPlanes); ++it)
	{
		auto& n = *it;

		n *= -1.f;
	}

}




