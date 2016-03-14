//
//  WorldEntity.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "WorldEntity.h"

#include "Material.h"


void WorldEntity::OnRender(const GameTime& time)
{
	auto mat = GetFirst<Material>();

	if (nullptr == mat)
		return;

	mat->Bind();
	mat->SetUniform("World", Transform->GetMatrix());
}