//
//  Transform.hpp
//  GameEngine
//
//  Created by David Lively on 2/17/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Common.h"
#include "Vectors.h"
#include "Matrix.h"
#include "GameObject.h"

class Transform
{
public:
    Vector3 Translation;
    Vector3 Rotation;
    Vector3 Scale;
    
    Transform()
    {
        Scale = Vector3(1,1,1);
        
    }
private:
};

#endif /* Transform_hpp */
