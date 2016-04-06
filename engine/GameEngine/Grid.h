//
//  Ship.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef GRID_H
#define GRID_H

#include "Common.h"
#include "Drawable.h"

class Grid : public Drawable
{
public:
	Grid(const std::string& name) : Drawable(name)
	{

	}

	Grid() : Grid("grid")
	{

	}



	bool OnInitialize() override;

	void OnUpdate(const GameTime& time)  override;

};


#endif /* Ship_hpp */
