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
#include "Explodable.h"
#include "Mesh.h"
#include "InputHandler.h"

class Ship : public Explodable
{
public:
	bool ScoreboardIndicator = false;
	float TimeUntilCanFire = 0.f;
	const float FireIntervalSec = 0.25f;

	Ship(const std::string& name) : Explodable(name)
	{

	}

	Ship() : Ship("ship")
	{

	}

	bool enableBreak = false;

	void OnRender(const GameTime& time) override
	{
		Log::Info << "Rendering ship " << Name << std::endl;
		Explodable::OnRender(time);
	}

	bool OnInitialize() override;
	void OnPreUpdate(const  GameTime& time) override;

	bool CanFire();
	bool Fire();

	void EnableInput(bool isEnabled)
	{
		m_inputEnabled = isEnabled;
		if (nullptr != m_input)
			m_input->Enabled = m_inputEnabled;
	}

	bool IsRespawning = false;



private:
	InputHandler* m_input = nullptr;

	void ConfigureInput();

	void CreateShipMesh();
	void CreateHelpers();

	bool m_inputEnabled = false;

};


#endif /* Ship_hpp */
