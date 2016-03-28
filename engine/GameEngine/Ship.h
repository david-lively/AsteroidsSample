//
//  Ship.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef SHIP_H
#define SHIP_H

#include "Common.h"
#include "WorldEntity.h"
#include "Mesh.h"

class Ship : public WorldEntity
{
public:

	float TimeUntilCanFire = 0.f;
	const float FireIntervalSec = 0.2f;

    Ship()
    {
        
    }
    
    bool OnInitialize() override;
	void OnPreUpdate(const  GameTime& time) override;
	bool CanFire();
	bool Fire();

    
private:
    Mesh* m_mesh;
    Material* m_material;
    
    void ConfigureInput();

	void CreateShipMesh();
	void CreateHelpers();
    
};


#endif /* Ship_hpp */
