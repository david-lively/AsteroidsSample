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
#include "WorldEntity.h"
#include "Mesh.h"

class Grid : public WorldEntity
{
public:
    Grid()
    {
        
    }
    
    bool OnInitialize() override;
    
private:
    Mesh* m_mesh;
    Material* m_material;
    
    
};


#endif /* Ship_hpp */
