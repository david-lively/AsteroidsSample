//
//  Draw.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef Draw_H
#define Draw_H

#include "Explodable.h"
#include "Mesh.h"
#include "InputHandler.h"

enum class PrimitiveType
{
	Line
	,Circle
};

class Draw : public Explodable
{
public:
	PrimitiveType DrawAs = PrimitiveType::Circle;

	Draw() : Draw("Draw")
    {
        
    }

	Draw(const std::string& name) : Explodable(name)
	{

	}

    bool OnInitialize() override;
    
	void OnRender(const GameTime& time) override;

};


#endif /* Draw_h */

