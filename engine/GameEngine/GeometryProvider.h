//
//  GeometryProvider.hpp
//  GameEngine
//
//  Created by David Lively on 2/15/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef GEOMETRYPROVIDER_H
#define GEOMETRYPROVIDER_H

#include <vector>

#include "Common.h"
#include "Vectors.h"

class GeometryProvider
{
public:
    static void Cube(std::vector<Vector3> &vectors, std::vector<GLushort> &indices);

    

private:
    
};

#endif /* GeometryProvider_hpp */
