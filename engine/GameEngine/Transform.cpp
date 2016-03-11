//
//  Transform.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Transform.h"


void Transform::Wrap(const Vector3& flags)
{
    float scale = 0.99f;
    
    auto velocity = Translation - m_previousTranslation;
    
    if (flags.X < 1)
    {
        Translation.X = -Translation.X * scale;
    }
    
    if (flags.Y < 1)
    {
        Translation.Y = -Translation.Y * scale;
    }
    
    m_previousTranslation = Translation - velocity;
}

void Transform::SetRotation(const Vector3& rotation)
{
    m_previousRotation = (m_previousRotation - Rotation) + rotation;
    Rotation = rotation;
}

void Transform::Stop()
{
    m_previousTranslation = Translation;
    m_previousRotation = Rotation;
    
}

void Transform::Reset()
{
    Translation = Vector3(0);
    m_previousTranslation = Translation;
    
    Rotation = Vector3(0);
    m_previousRotation = Rotation;
}

BoundingBox Transform::TransformAABB(const BoundingBox& bounds)
{
    BoundingBox bb;
    
    auto m = GetMatrix();
    
    
    auto mn = m.Transform(bounds.Min);
    auto mx = m.Transform(bounds.Max);
    
    bb = BoundingBox::FromMinMax(mn, mx);
    
    return bb;
}






