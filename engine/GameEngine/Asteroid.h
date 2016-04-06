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

class Asteroid : public Explodable
{
public:
	InputHandler& Input;

	Asteroid() : Asteroid("asteroid")
    {
        
    }

	Asteroid(const std::string& name) : Explodable(name)
		, Input(Create<InputHandler>("asteroid.input"))
	{

	}

	bool TwoD = true;
    
    bool OnInitialize() override;
    
	void OnRender(const GameTime& time) override;
	void OnPreRender(const GameTime& time) override;
    void OnUpdate(const GameTime& time) override;
    
};


#endif /* Asteroid_h */

