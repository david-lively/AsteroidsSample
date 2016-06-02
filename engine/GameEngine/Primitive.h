//
//  Primitive.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Explodable.h"
#include "Mesh.h"
#include "InputHandler.h"

enum class PrimitiveType
{
	Line
	,Circle
};

class Primitive : public Explodable
{
public:
	PrimitiveType DrawAs = PrimitiveType::Circle;

	Primitive() : Primitive("Primitive")
    {
        
    }

	Primitive(const std::string& name) : Explodable(name)
	{

	}

    bool OnInitialize() override;
    
	void OnRender(const GameTime& time) override;

};


#endif /* Primitive_h */

