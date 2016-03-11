//
//  Vectors.cpp
//  GameEngine
//
//  Created by David Lively on 2/10/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Vectors.h"


std::ostream& operator<<(std::ostream& os, const Vector4& vec)
{
    os << "(" << vec.X << " " << vec.Y << " " << vec.Z << " " << vec.W << ")";
    
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec)
{
    os << "(" << vec.X << " " << vec.Y << " " << vec.Z << ")";
    
    return os;
}

