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

	float ExplosionFactor = 0.f;
	float ExplosionTime = 0.f;
	float ExplosionDuration = 1.f;
	bool IsExploding = false;

	Ship()
	{

	}

	bool OnInitialize() override;
	void OnPreUpdate(const  GameTime& time) override;
	void OnRender(const GameTime& time) override;

	bool CanFire();
	bool Fire();

	void Explode(const GameTime& time, const float duration = -1.f);

	void EnableInput(bool isEnabled)
	{
		m_inputEnabled = isEnabled;
		if (nullptr != m_input)
			m_input->Enabled = m_inputEnabled;
	}

private:
	Mesh* m_mesh;
	Material* m_material;

	InputHandler* m_input;

	void ConfigureInput();

	void CreateShipMesh();
	void CreateHelpers();

	bool m_inputEnabled = false;

};


#endif /* Ship_hpp */
