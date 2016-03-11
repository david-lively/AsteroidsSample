//
//  GeometryProvider.hpp
//  GameEngine
//
//  Created by David Lively on 3/8/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef GEOMETRYPROVIDER_H
#define GEOMETRYPROVIDER_H

#include <vector>
#include "Vectors.h"
#include "Common.h"

class GeometryProvider
{
public:
    /// abstract
    virtual ~GeometryProvider() = 0;
    
    
    static void FitToUnitCube(std::vector<Vector3>& vectors);
    static void LineGrid(std::vector<Vector3>& vertices, int cellCount);
    static void Sphere(std::vector<Vector3>& vertices, std::vector<GLushort>& indices);
    
private:
    
};

#endif /* GeometryProvider_hpp */
