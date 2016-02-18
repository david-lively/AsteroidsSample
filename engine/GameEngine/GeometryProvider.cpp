//
//  GeometryProvider.cpp
//  GameEngine
//
//  Created by David Lively on 2/15/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "GeometryProvider.h"

#include <vector>
using namespace std;

#include "Vectors.h"




void GeometryProvider::Cube(vector<Vector3> &vectors, vector<GLushort> &indices)
{
    float extents = 0.5;
    std::array<float,24> corners =
    {
        -extents,extents,extents
        ,
        extents,extents,extents
        ,
        extents,-extents,extents
        ,
        -extents,-extents,extents
        ,
        -extents,extents,-extents
        ,
        extents,extents,-extents
        ,
        extents,-extents,-extents
        ,
        -extents,-extents,-extents
    };
    
    
    std::array<GLushort,36> triangles = {
        0,1,2
        ,
        0,2,3
        ,
        3,2,6
        ,
        3,6,7
        ,
        4,0,3
        ,
        4,3,7
        ,
        5,4,7
        ,
        5,7,6
        ,
        1,5,6
        ,
        1,6,2
        ,
        4,5,1
        ,
        4,1,0
    };
    
    
    unsigned long startIndex = vectors.size();
    vectors.reserve(vectors.size() + corners.size() / 3);
    
    for(int i = 0; i < corners.size(); i+=3)
    {
        vectors.push_back(Vector3(corners[i],corners[i+1],corners[i+2]));
    }
    
    indices.reserve(indices.size() + triangles.size());
    
    for(int i = 0; i < triangles.size(); i++)
    {
        indices.push_back(triangles[i] + startIndex);
    }
}


