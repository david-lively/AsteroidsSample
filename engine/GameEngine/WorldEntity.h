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


class WorldEntity : public GameObject
{
public:
    Transform* Transform;
    BoundingBox Bounds;
    
    
    WorldEntity(const std::string& name) : GameObject(name)
    {
        Transform = &Create<class Transform>("asdf");
    }
    
    WorldEntity() : WorldEntity("unknown.worldentity")
    {
        
    }
    
    
};


#endif /* WorldEntity_hpp */
