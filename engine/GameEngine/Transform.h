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
#include "Vectors.h"
#include "Matrix.h"
#include "Bounds.h"
#include "GameObject.h"

enum class TransformSequence
{
	ScaleRotateTranslate
	,ScaleTranslateRotate
};

class Transform : public GameObject
{
public:
    Vector3 Translation;
    Vector3 Rotation;
    Vector3 Scale;

	TransformSequence Sequence = TransformSequence::ScaleRotateTranslate;
    
    float TranslationDrag = 0.05f;
	float RotationDrag = 0.05f;
    
	Transform() : Transform("transform")
    {
		Scale = Vector3::One;
    }

	Transform(const std::string& name) : GameObject(name), Scale(Vector3::One)
	{

	}

	Vector3 Velocity()
	{
		return Translation - m_previousTranslation;
	}

	Transform& CopyFrom(const Transform& other)
	{
		Translation = other.Translation;
		Rotation = other.Translation;
		Scale = other.Scale;

		TranslationDrag = other.TranslationDrag;
		RotationDrag = other.RotationDrag;

		m_previousTranslation = Translation;
		m_previousRotation = Rotation;

		return *this;
	}
    
	Matrix GetMatrix();
    
    /// transformed Z axis
	Vector3 Forward();
    
	void Push(const float x, const float y, const float z);
	void Push(const Vector3& dir);
    
    /// move to a new position, but do not add velocity.
	void Move(const Vector3& newPosition);
	void Move(const float x, const float y, const float z);

    
	void Spin(const Vector3& theta);
	void Spin(const  float x, const float y, const float z);

	void Bounce(const Vector3& dir);

	void OnUpdate(const GameTime& time) override;
    
    void SetRotation(const Vector3& rotation);

    /// stop all motion and rotation
    void Stop();
    
    /// Reset rotation and translation to zero, and cancel spin and movement.
    void Reset();
    
    BoundingBox TransformAABB(const BoundingBox& bounds);
	BoundingSphere TransformSphere(const BoundingSphere& bounds);
    

private:
    Vector3 m_previousTranslation;
    Vector3 m_previousRotation;

	float m_previousFrameTime = -1;
    
    
    
};


#endif /* Transform_hpp */
