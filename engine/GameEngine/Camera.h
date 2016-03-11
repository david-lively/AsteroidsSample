//
//  Camera.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "WorldEntity.h"
#include "Matrix.h"
#include "Vectors.h"
#include "Bounds.h"


class Camera : public WorldEntity
{
public:
    float FieldOfView;
    float ZNear;
    float ZFar;

    Vector3 Left;
    Vector3 Right;
    Vector3 Top;
    Vector3 Bottom;
    Vector3 Near;
    Vector3 Far;
    
    Camera() : FieldOfView(TO_RADIANS(90)), ZNear(1), ZFar(1000)
    {
    }

    Matrix GetProjectionMatrix();
    Matrix GetViewMatrix();
    Matrix GetViewProjectionMatrix();
    
    bool ContainsSphere(const Vector3& center, const float radius);
    bool ContainsSphere(const Vector3& center, const float radius, Vector3& containment);
    bool ContainsPoint(const Vector3& point);
    
private:
    
    float m_previousAspectRatio = 0;

    Matrix m_projectionMatrix;
    Matrix m_viewMatrix;
    
    void OnUpdate(const GameTime& time) override;
    
    void BuildFrustumPlanes();
    
};


#endif /* Camera_h */
