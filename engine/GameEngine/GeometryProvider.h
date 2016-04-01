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
    static void Icosahedron(std::vector<Vector3>& vertices, std::vector<GLushort>& indices);
    static void Tessellate(std::vector<Vector3>& vertices, std::vector<GLushort>& indices, int levels);
	static void Spherize(std::vector<Vector3>& vertices);

	static void Quad(std::vector<Vector3>& vertices, std::vector<GLushort>& indices, const float width, const float height);

	static void FindExtents(const std::vector<Vector3>& vertices, Vector3& boxMin, Vector3& boxMax);
	static const Vector3 FindCenter(std::vector<Vector3>& vertices);
	static void Circle(std::vector<Vector3>& vertices, std::vector<GLushort>& indices, Vector3 center, float radius, int segments, bool inXZ=false);
	static std::vector<Vector3> ToVectors(std::vector<float>& coordinates);

	static void Arrow(std::vector<Vector3>& vertices, std::vector<GLushort>& indices, const float width, const float height);
	/// offset all vertices along their direction from the object center.
	static void Noisify(std::vector<Vector3>& vertices, float scale, float amplitude);

	static void Cone(std::vector<Vector3>& vertices, std::vector<GLushort>& indices, const float  height, const float radius, const int sides, const bool generateBase = true);


private:
    
};

#endif /* GeometryProvider_hpp */
