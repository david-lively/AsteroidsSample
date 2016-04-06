//
//  Explodable.h
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef EXPLODABLE_H
#define EXPLODABLE_H

#include "Common.h"
#include "Drawable.h"
#include "Mesh.h"

class Explodable : public Drawable
{
public:

	float ExplosionFactor = 0.f;
	float ExplosionTime = 0.f;
	float ExplosionDuration = 1.f;
	bool IsExploding = false;

	Explodable(const std::string& name) : Drawable(name)
	{

	}


	Explodable() : Explodable("explodable")
	{

	}

	void OnRender(const GameTime& time) override;

	void Explode(const GameTime& time, const float duration = -1.f);
};


#endif /* Ship_hpp */
