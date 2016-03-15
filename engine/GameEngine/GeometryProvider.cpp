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
#include <map>

using namespace std;

GeometryProvider::~GeometryProvider()
{
    /// pure virtual destructor, abstract class.
}


void GeometryProvider::Spherize(std::vector<Vector3>& vertices)
{
	FitToUnitCube(vertices);

	for (auto it = begin(vertices); it != end(vertices);  ++it)
		it->Normalize();


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
    
    for(int i = 0; i <= cellCount; ++i)
    {
        float vary = i * scale - 0.5f;
        
        vertices.push_back(Vector3(vary,0.5f,0));
        vertices.push_back(Vector3(vary, -0.5f, 0));
        
        vertices.push_back(Vector3(-0.5f, vary, 0));
        vertices.push_back(Vector3(0.5f, vary, 0));
        
    }
    
    
}

void GeometryProvider::Icosahedron(vector<Vector3>& vertices, vector<GLushort>& indices)
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



string GetMidpointName(int a, int b)
{
    return to_string(min(a,b)) + "-" + to_string(max(a,b));
}

GLushort GetIndexOfMidpoint(int a, int b, std::vector<Vector3>& vertices, std::map<string,int>& edges)
{
    auto name = GetMidpointName(a, b);
    
	int midpointIndex = -1;

	if (edges.count(name) == 0)
	{
		midpointIndex = (int)vertices.size();

		auto newVertex = (vertices[a] + vertices[b]) * 0.5f;

		edges[name] = (GLushort)midpointIndex;
		vertices.push_back(newVertex);
	}
	else
		midpointIndex = edges[name];

	return midpointIndex;
}

/*
 v0
 /\
 /  \
 /    \
 /      \
 /        \
 m20- - - - m01
 /  \      /   \
 /    \    /     \
 /      \  /       \
 /        \/         \
 v2 - - - -m12 - - -  v1
 
 
 faces:
 v01,m01,m20
 m01,v1,m12
 m20,m12,v2
 m20,m01,m12
 
 */
void GeometryProvider::Tessellate(vector<Vector3>& vertices, std::vector<GLushort>& sourceIndices, int levels)
{
    if (levels <= 0)
        return;
    
    vector<GLushort> indices(sourceIndices);
    map<string,int> edges;
    
    vector<GLushort> newIndices;
    
    for(int i = 0; i < indices.size(); i += 3)
    {
        GLushort i0 = indices[i];
        GLushort i1 = indices[i + 1];
        GLushort i2 = indices[i + 2];
        
        auto m01 = GetIndexOfMidpoint(i0, i1, vertices, edges);
		auto m12 = GetIndexOfMidpoint(i1, i2, vertices, edges);
		auto m20 = GetIndexOfMidpoint(i2, i0, vertices, edges);
        
        std::vector<GLushort> newFaces =
        {
            i0, m01, m20
            ,
            m01, i1, m12
            ,
            m20, m12, i2
            ,
            m20, m01, m12
        };
        
        newIndices.insert(end(newIndices), begin(newFaces), end(newFaces));
    }

    /// wonder if the compiler does tail recursion
    Tessellate(vertices, newIndices, --levels);

	sourceIndices = newIndices;
}



