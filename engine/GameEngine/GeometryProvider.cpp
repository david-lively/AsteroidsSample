//
//  GeometryProvider.cpp
//  GameEngine
//
//  Created by David Lively on 3/8/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "GeometryProvider.h"
#include "Common.h"

#include <vector>

using namespace std;

GeometryProvider::~GeometryProvider()
{
    /// pure virtual destructor, abstract class.
}


void GeometryProvider::FitToUnitCube(std::vector<Vector3>& vectors)
{
    /// bounds
    auto vmin = vectors[0];
    auto vmax = vectors[0];
    
    for(int i = 1; i < vectors.size(); ++i)
    {
        vmin = Vector3::Min(vectors[i],vmin);
        vmax = Vector3::Max(vectors[i],vmax);
    }
    
    float maxSide = max(max(vmax.X-vmin.X, vmax.Y-vmin.Y), vmax.Z - vmin.Z);
    
    for(auto it = begin(vectors); it != end(vectors); ++it)
    {
        auto& v = *it;
        
        v.X = (v.X - vmin.X) / maxSide - 0.5f;
        v.Y = (v.Y - vmin.Y) / maxSide - 0.5f;
        v.Z = (v.Z - vmin.Z) / maxSide - 0.5f;
    }
    
}

void GeometryProvider::LineGrid(vector<Vector3>& vertices, int cellCount = 4)
{
    float scale = 1.f / cellCount;
    
    for(auto i = 0; i <= cellCount; ++i)
    {
        float vary = i * scale - 0.5f;
        vertices.push_back(Vector3(vary,0.5f,0));
        vertices.push_back(Vector3(vary, -0.5f, 0));
        
        vertices.push_back(Vector3(-0.5f, vary, 0));
        vertices.push_back(Vector3(0.5f, vary, 0));
        
    }
    
    
}

void GeometryProvider::Sphere(vector<Vector3>& vertices, vector<GLushort>& indices)
{
    float X = 0.525731112119133606f;
    float Z = 0.850650808352039932f;
    
    vector<Vector3> icosahedronVerts =
    {
        Vector3(-X, 0.f, Z),
        Vector3(X, 0.f, Z),
        Vector3(-X, 0.f, -Z),
        Vector3(X, 0.f, -Z),
        Vector3(0.f, Z, X),
        Vector3(0.f, Z, -X),
        Vector3(0.f, -Z, X),
        Vector3(0.f, -Z, -X),
        Vector3(Z, X, 0.f),
        Vector3(-Z, X, 0.f),
        Vector3(Z, -X, 0.f),
        Vector3(-Z, -X, 0.f)
    };
    
    vertices.insert(end(vertices),begin(icosahedronVerts),end(icosahedronVerts));

    vector<GLushort> icosahedronIndices =
    {
        0,4,1,
        0,9,4,
        9,5,4,
        4,5,8,
        4,8,1,
        8,10,1,
        8,3,10,
        5,3,8,
        5,2,3,
        2,7,3,
        7,10,3,
        7,6,10,
        7,11,6,
        11,0,6,
        0,1,6,
        6,1,10,
        9,0,11,
        9,11,2,
        9,2,5,
        7,2,11
    };
    
    indices.insert(end(indices), begin(icosahedronIndices), end(icosahedronIndices));
}


/*
 indices.AddRange(
 new int[]
 {
 0,4,1,
 0,9,4,
 9,5,4,
 4,5,8,
 4,8,1,
 8,10,1,
 8,3,10,
 5,3,8,
 5,2,3,
 2,7,3,
 7,10,3,
 7,6,10,
 7,11,6,
 11,0,6,
 0,1,6,
 6,1,10,
 9,0,11,
 9,11,2,
 9,2,5,
 7,2,11
 }
 .Select(i => i + vertices.Count)
 );
 
 var X = 0.525731112119133606f;
 var Z = 0.850650808352039932f;
 
 vertices.AddRange(
 new[]
 {
 new Vector3(-X, 0f, Z),
 new Vector3(X, 0f, Z),
 new Vector3(-X, 0f, -Z),
 new Vector3(X, 0f, -Z),
 new Vector3(0f, Z, X),
 new Vector3(0f, Z, -X),
 new Vector3(0f, -Z, X),
 new Vector3(0f, -Z, -X),
 new Vector3(Z, X, 0f),
 new Vector3(-Z, X, 0f),
 new Vector3(Z, -X, 0f),
 new Vector3(-Z, -X, 0f)
 }
 );
 */




