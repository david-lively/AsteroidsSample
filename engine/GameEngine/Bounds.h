//
//  BoundingBox.h
//  GameEngine
//
//  Created by David Lively on 3/10/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#pragma once

#include "Vectors.h"
#include "Common.h"

#include <vector>
#include <cmath>

enum class IntersectionType
{
    Disjoint
    ,
    Contains
    ,
    Intersects
};



struct BoundingBox
{
    Vector3 Min;
    Vector3 Max;

	BoundingBox()
	{

	}

	BoundingBox(const Vector3& min, const Vector3& max) : Min(min), Max(max)
	{

	}
    
    static BoundingBox FromMinMax(const Vector3& min, const Vector3& max)
    {
        BoundingBox bb;
        bb.Min = Vector3::Min(min,max);
        bb.Max = Vector3::Max(min,max);
        
        return bb;
    }
    
    
    static BoundingBox FromVectors(const std::vector<Vector3>& vectors)
    {
        BoundingBox bb;
        
        bb.Min = vectors[0];
        bb.Max = vectors[0];
        
        for(int i = 0; i < vectors.size(); ++i)
        {
            bb.Min = Vector3::Min(vectors[i], bb.Min);
            bb.Max = Vector3::Max(vectors[i], bb.Max);
        }
        
        return bb;
    }
    
    static BoundingBox FromFloats(const std::vector<float>& coordinates)
    {
        BoundingBox bb;
        
        Vector3 mn;
        Vector3 mx;
        
        mn = Vector3(coordinates[0],coordinates[1],coordinates[2]);
        mx = mn;
        
        
        for(int i = 3; i < coordinates.size(); i += 3)
        {
            float x = coordinates[i];
            float y = coordinates[i+1];
            float z = coordinates[i+2];
            
            mn.X = min(mn.X,x);
            mn.Y = min(mn.Y,y);
            mn.Z = min(mn.Z,z);
            
            mx.X = max(mx.X,x);
            mx.Y = max(mx.Y,y);
            mx.Z = max(mx.Z,z);
        }
        
        bb.Min = mn;
        bb.Max = mx;
        
        return bb;
    }
    
    IntersectionType Intersects(const BoundingBox& other)
    {
        if (other.Max.X < Min.X || other.Min.X > Max.X
            || other.Max.Y < Min.Y || other.Min.Y > Max.Y
            || other.Max.Z < Min.Z || other.Min.Z > Max.Z
            )
            return IntersectionType::Disjoint;
        
        else
            return IntersectionType::Intersects;
        
    }
    
    
};

class BoundingSphere
{
public:
	BoundingSphere(const Vector3& center, const float radius) : 
		Center(center), Radius(radius)
	{
		
	}

	BoundingSphere()
	{

	}

	Vector3 Center;
	float Radius;

	bool Intersects(const BoundingSphere& other)
	{
		Vector3 delta = (other.Center - Center);

		float distance = delta.Length();
	
		bool result = distance < (Radius + other.Radius);

		return result;
	}

	static BoundingSphere FromVectors(const std::vector<Vector3>& vectors)
	{
		Vector3 mn = vectors[0];
		Vector3 mx = vectors[0];

		for (int i = 1; i < vectors.size(); ++i)
		{
			mn = Vector3::Min(mn, vectors[i]);
			mx = Vector3::Max(mx, vectors[i]);
		}

		float maxDim = max(max(mx.X - mn.X, mx.Y - mn.Y), mx.Z - mn.Z);

		return BoundingSphere((mn + mx) * 0.5f, maxDim * 0.5f);
	}

};



