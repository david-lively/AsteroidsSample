//  Explodable.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Common.h"
#include "Explodable.h"
#include <vector>
#include <cmath>

using namespace std;

void Explodable::OnRender(const GameTime& time)
{
	Material.Bind();

	Uniforms.SetUniform("ExplosionFactor", ExplosionFactor);
	Uniforms.SetUniform("IsExploding", IsExploding ? 1.f : 0.f);

	Drawable::OnRender(time);
}


void Explodable::Explode(const GameTime& time, const float duration)
{
	ExplosionTime = 0.f;
	if (duration > 0)
		ExplosionDuration = duration;

	Vector3 spin((rand() % 10) / 10.f, (rand() % 10) / 10.f, (rand() % 10) / 10.f);

	Transform.Spin(spin * 0.1f);

	IsExploding = true;
}





