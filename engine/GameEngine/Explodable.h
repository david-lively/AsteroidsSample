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

#include <vector>
#include <functional>

class Explodable : public Drawable
{
public:
	float ExplosionFactor = 0.f;
	float ExplosionTime = 0.f;
	float ExplosionDuration = 1.f;
	float ExplosionSpeed = 1.f;
	bool IsExploding = false;

	bool Broken;

	// normals for breaks 
	std::vector<Vector3> BreakPlanes;


	Explodable(const std::string& name) : Drawable(name)
	{
	}


	Explodable() : Explodable("explodable")
	{

	}

	void OnRender(const GameTime& time) override;
	void OnPreUpdate(const GameTime& time) override;
	virtual void Explode(const GameTime& time, const float duration = -1.f);

	void Break(const GameTime& time, const Vector3& impactPoint, bool pushBackward = false);
	virtual void Reset();

	GameTimeEvent OnExploded;

private:

};


#endif /* Ship_hpp */
