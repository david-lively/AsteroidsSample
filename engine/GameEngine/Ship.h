//
//  Ship.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef SHIP_H
#define SHIP_H

#include "Common.h"
#include "WorldEntity.h"
#include "Mesh.h"

class Ship : public WorldEntity
{
public:

	float TimeUntilCanFire = 0.f;
	const float FireIntervalSec = 0.5f;

	float ExplodeTimeRemaining = 0.f;
	float ExplodeScale = 1.f;

	bool Exploding() { return ExplodeTimeRemaining > 0; }

	Vector3 ExplodeMove;


	Ship()
	{

	}

	bool OnInitialize() override;
	void OnPreUpdate(const  GameTime& time) override;
	void OnRender(const GameTime& time) override;

	bool CanFire();
	bool Fire();

	void Explode(const GameTime& time, const float explosionTime);

	void EnableInput(bool isEnabled)
	{
		if (nullptr != m_input)
			m_input->Enabled = isEnabled;
	}

private:
	Mesh* m_mesh;
	Material* m_material;

	InputHandler* m_input;

	void ConfigureInput();

	void CreateShipMesh();
	void CreateHelpers();

};


#endif /* Ship_hpp */
