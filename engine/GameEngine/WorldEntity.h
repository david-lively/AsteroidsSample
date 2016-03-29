//
//  WorldEntity.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef WORLDENTITY_H
#define WORLDENTITY_H

#include "GameObject.h"
#include "Transform.h"
#include "InputHandler.h"
#include "Bounds.h"

#include <string>

enum class FrustumAction
{
	None
	,
	Wrap
	,
	Recycle
	,
	Delete
};

class WorldEntity : public GameObject
{
public:
    Transform* Transform;
    BoundingBox Bounds;

	BoundingSphere Sphere;

	FrustumAction OnExitFrustum = FrustumAction::Wrap;
    
    
    WorldEntity(const std::string& name) : GameObject(name)
    {
        Transform = &Create<class Transform>(name + "-transform");
    }
    
    WorldEntity() : WorldEntity("unknown.worldentity")
    {
        
    }

   
};


#endif /* WorldEntity_h */
