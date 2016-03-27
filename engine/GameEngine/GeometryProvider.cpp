//
//  GeometryProvider.cpp
//  GameEngine
//
//  Created by David Lively on 3/8/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "GeometryProvider.h"
#include "Common.h"
#include "Bounds.h"
#include "SimplexNoise.h"

#include <vector>
#include <map>
#include <cassert>
#include <random>

using namespace std;

GeometryProvider::~GeometryProvider()
{
    /// pure virtual destructor, abstract class.
}



void GeometryProvider::FindExtents(const std::vector<Vector3>& vertices, Vector3& boxMin, Vector3& boxMax)
{
	if (vertices.size() == 0)
	{
		boxMin = Vector3(0);
		boxMax = Vector3(0);

		return;

	}


	boxMin = vertices[0];
	boxMax = vertices[0];

	for (int i = 1; i < vertices.size(); ++i)
	{
		boxMin = Vector3::Min(boxMin, vertices[i]);
		boxMax = Vector3::Max(boxMax, vertices[i]);
	}

}

const Vector3 GeometryProvider::FindCenter(std::vector<Vector3>& vertices)
{
	Vector3 boxMin, boxMax;

	FindExtents(vertices, boxMin, boxMax);

	return (boxMin + boxMax)  * 0.5f;
}


void GeometryProvider::Spherize(std::vector<Vector3>& vertices)
{
	auto center = FindCenter(vertices);

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


vector<Vector3> GeometryProvider::ToVectors(std::vector<float>& coordinates)
{
	vector<Vector3> vertices;

	for (int i = 0; i < coordinates.size(); i += 3)
	{
		vertices.push_back(Vector3(coordinates[i], coordinates[i + 1], coordinates[i + 2]));
	}

	return vertices;
}


void GeometryProvider::Circle(std::vector<Vector3>& vertices, std::vector<GLushort>& indices, Vector3 center, float radius, int segments, bool inXZ)
{
	GLushort firstIndex = (GLushort)vertices.size();

	float dTheta = TO_RADIANS(360.f / segments);

	for (float theta = 0; theta <= TO_RADIANS(360); theta += dTheta)
	{
		
		float x = cosf(theta) * radius;
		float y = sinf(theta) * radius;

		Vector3 v(x, y, 0);

		if (inXZ)
		{
			v.Z = v.Y;
			v.Y = 0;
		}

		vertices.push_back(v + center);
	}

	GLushort lastIndex = (GLushort)(vertices.size() - 1);

	for (int i = firstIndex; i < lastIndex; ++i)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	indices.push_back(lastIndex);
	indices.push_back(firstIndex);
}

void GeometryProvider::Quad(vector<Vector3>& vertices, vector<GLushort>& indices, const float width, const float height)
{
	// half-space sizes
	float hw = width * 0.5f;
	float hh = height * 0.5f;

	int startIndex = vertices.size();

	vertices.reserve(vertices.size() + 4);

	array<float,4 * 3> coordinates =
	{
		-hw, hh, 0
		,
		hw, hh, 0
		,
		hw, -hh, 0
		,
		-hw, -hh, 0
	};

	for (int i = 0; i < coordinates.size(); i += 3)
		vertices.push_back(Vector3(coordinates[i], coordinates[i + 1], coordinates[i + 2]));

	for (int i = 0; i <= coordinates.size(); ++i)
	{
		indices.push_back(startIndex + i);
	}
}

void GeometryProvider::Arrow(std::vector<Vector3>& vertices, std::vector<GLushort>& indices, const float width, const float height)
{
	int startIndex = (int)vertices.size();


	float halfWidth = width * 0.5f;
	float headHeight = height * 0.25f;

	vector<float> coordinates =
	{
		0,0,0
		,
		0,height,0
		,
		halfWidth, height - headHeight, 0
		,
		-halfWidth, height - headHeight,0
	};

	vertices.reserve(vertices.size() + coordinates.size() / 3);

	for (int i = 0; i < coordinates.size(); i += 3)
	{
		vertices.push_back(Vector3(coordinates[i], coordinates[i + 1], coordinates[i + 2]));
	}

	vector<GLushort> newIndices =
	{
		0, 1
		,
		1, 2
		,
		1, 3
	};

	indices.reserve(indices.size() + newIndices.size());
	
	for (int i = 0; i < newIndices.size(); ++i)
	{
		indices.push_back(newIndices[i] + startIndex);
	}

}


/// offset all vertices along their direction from the object center.
void GeometryProvider::Noisify(std::vector<Vector3>& vertices, float noiseScale)
{
	Vector3 noiseCenter((rand() % 1000) / 10.f, (rand() % 1000) / 10.f, (rand() % 1000) / 10.f);;

	for (auto& vertex : vertices)
	{
		float noise = SimplexNoise::noise(vertex.X, vertex.Y);

		vertex += vertex.Normalized() * noise *noiseScale;
	}
}

void GeometryProvider::Cone(std::vector<Vector3>& vertices, std::vector<GLushort>& indices, const float height, const float radius, const int sides)
{
	vertices.push_back(Vector3(0, height / 2.f, 0));

	Circle(vertices, indices, Vector3(0, -height / 2.f, 0), radius, sides, true);
	
	indices.clear();


	for (int i = 1; i < vertices.size(); ++i)
	{
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back((i + 1) % vertices.size());
	}

}



