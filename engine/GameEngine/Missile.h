//
//  Missile.h
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef MISSILE_H
#define MISSILE_H

#include "Common.h"
#include "WorldEntity.h"
#include "Mesh.h"

class Missile : public WorldEntity
{
public:
	Missile()
    {
		OnExitFrustum = FrustumAction::Recycle;
    }

    bool OnInitialize() override;

    
private:
    Mesh* m_mesh;
    Material* m_material;
};


#endif /* Missile_h */

