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
    
    Matrix GetMatrix()
    {
        auto mt = Matrix::CreateTranslation(Translation);
        auto mr = Matrix::CreateRotation(Rotation);
        auto ms = Matrix::CreateScale(Scale);
        
        auto result = mr * mt * ms;
        
        return result;
    }
    
    
    /// transformed Z axis
    Vector3 Forward()
    {
        auto m = GetMatrix();
        
        
        return Vector3(m.m20, m.m21, m.m22);
    }
    
    void Push(const Vector3& dir)
    {

        Translation += dir;
    }
    
    /// move to a new position, but do not add velocity.
    void Move(const Vector3& newPosition)
    {
        auto velocity = Translation - m_previousTranslation;
        Translation = newPosition;
        m_previousTranslation = newPosition - velocity;
    }
    
    void Spin(const Vector3& theta)
    {
        Rotation += theta;
    }
    
    void OnUpdate(const GameTime& time) override
    {
		//static float lastSeconds = 1.66e-3f;

        auto velocity = (Translation - m_previousTranslation) * (1 - Drag);
        
		//velocity *= time.ElapsedSeconds() / lastSeconds;

        m_previousTranslation = Translation;
        Translation += velocity;
        
        auto spin = (Rotation - m_previousRotation) * (1 - Drag);
        m_previousRotation = Rotation;
        Rotation += spin;

		//lastSeconds = time.ElapsedSeconds();
    }
    
    /// move to the given coordinates maintaining current velocity.
    void Wrap(const Vector3& flags);
    void SetRotation(const Vector3& rotation);

    /// stop all motion and rotation
    void Stop();
    
    /// Reset rotation and translation to zero, and cancel spin and movement.
    void Reset();
    
    BoundingBox TransformAABB(const BoundingBox& bounds);
    

private:
    Vector3 m_previousTranslation;
    Vector3 m_previousRotation;
    
    
    
};


#endif /* Transform_hpp */
