//
//  Asteroid.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef ASTEROID_H
#define ASTEROID_H

#include "Explodable.h"
#include "Mesh.h"
#include "InputHandler.h"
#include "Text.h"


enum class AsteroidMaterial
{
	Rock
	,Confetti
};

class Asteroid : public Explodable
{
public:
	NotifyEvent OnReset = nullptr;
	
	Text& Caption;
	/* What the asteroid is made of - for different explosion effects, mass etc.*/
	AsteroidMaterial MadeOf;
	int PointValue = 10;
	int BreaksRemaining = 2;

	int Level() { return BreakPlanes.size(); }

	InputHandler& Input;

	Asteroid() : Asteroid("asteroid") 
    {
        
    }

	Asteroid(const std::string& name) : Explodable(name), Input(Create<InputHandler>("asteroid-input")), Caption(Create<Text>("asteroid.caption"))
	{

	}


	bool TwoD = true;
    
    bool OnInitialize() override;
    
	void OnRender(const GameTime& time) override;
    void OnUpdate(const GameTime& time) override;
private:
};


#endif /* Asteroid_h */

