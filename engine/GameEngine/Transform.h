//
//  Transform.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Common.h"
#include "GameObject.h"
#include "Vectors.h"
#include "Matrix.h"
#include "Bounds.h"

class Transform : public GameObject
{
public:
    Vector3 Translation;
    Vector3 Rotation;
    Vector3 Scale;
    
    
    float Drag = 0.05f;
    
    Transform()
    {
        Scale.X = Scale.Y = Scale.Z = 1;
    }

	Transform& CopyFrom(const Transform& other)
	{
		Translation = other.Translation;
		Rotation = other.Translation;


		m_previousTranslation = Translation;
		m_previousRotation = Rotation;

		return *this;
	}
    
	//Matrix GetCombinedMatrix();

	Matrix GetMatrix();
    
    /// transformed Z axis
	Vector3 Forward();
    
	void Push(const float x, const float y, const float z);
	void Push(const Vector3& dir);
    
    /// move to a new position, but do not add velocity.
	void Move(const Vector3& newPosition);
	void Move(const float x, const float y, const float z);

    
	void Spin(const Vector3& theta);
    
	void OnUpdate(const GameTime& time) override;
    
    void SetRotation(const Vector3& rotation);

    /// stop all motion and rotation
    void Stop();
    
    /// Reset rotation and translation to zero, and cancel spin and movement.
    void Reset();
    
    BoundingBox TransformAABB(const BoundingBox& bounds);
    

private:
    Vector3 m_previousTranslation;
    Vector3 m_previousRotation;

	float m_previousFrameTime = -1;
    
    
    
};


#endif /* Transform_hpp */
