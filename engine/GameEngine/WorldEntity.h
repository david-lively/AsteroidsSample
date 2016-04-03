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
#include "Bounds.h"

#include <string>

enum class FrustumAction
{
	None
	,
	Wrap
	,
	Recycle
};

class WorldEntity : public GameObject
{
public:
	Transform* Transform = nullptr;
    BoundingSphere Bounds;

	//bool DrawBounds = false;

	FrustumAction OnExitFrustum = FrustumAction::Wrap;
    
    WorldEntity(const std::string& name) : GameObject(name)
    {
        Transform = &Create<class Transform>(name + "-transform");
    }
    
    WorldEntity() : WorldEntity("unknown.worldentity")
    {
        
    }

private:
	//Material* m_material = nullptr;
	//Mesh* m_mesh = nullptr;
   
};


#endif /* WorldEntity_h */
