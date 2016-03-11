//
//  Camera.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Game.h"
#include "Camera.h"

#include <iostream>

using namespace std;

void Camera::OnUpdate(const GameTime& time)
{
//    Transform->SetRotation(Vector3(TO_RADIANS(-80), 0, 0));
    
    WorldEntity::OnUpdate(time);
    /// check aspect
    int w, h;
    
    Game::GetFramebufferSize(&w, &h);
    
    float aspect = w * 1.f / h;
    
    if (aspect != m_previousAspectRatio)
    {
        m_projectionMatrix = Matrix::CreatePerspective(FieldOfView, aspect, ZNear, ZFar);
        m_previousAspectRatio = aspect;
    }
    
    BuildFrustumPlanes();
}


Matrix Camera::GetProjectionMatrix()
{
    return m_projectionMatrix;
}

Matrix Camera::GetViewMatrix()
{
    auto t = Transform->Translation * -1.f;
    auto r = Transform->Rotation * -1.f;
    
    m_viewMatrix = Matrix::CreateRotation(r) * Matrix::CreateTranslation(t);
    
    return m_viewMatrix;
    
}

Matrix Camera::GetViewProjectionMatrix()
{
    auto p = GetProjectionMatrix();
    auto v = GetViewMatrix();
    
    auto pv = p * v;
    
    return pv;
}

void Camera::BuildFrustumPlanes()
{
    auto viewProj = m_viewMatrix * m_projectionMatrix;
    
    auto rows = viewProj.GetAllCols();
    
    auto vl = rows[3] + rows[0];
    vl /= vl.W;
    Left = (Vector3)vl.Normalized();
    
    auto vr = rows[3] - rows[0];
    vr /= vr.W;
    Right = (Vector3)vr.Normalized();
    
    auto vt = rows[3] - rows[1];
    vt /= vt.W;
    Top = (Vector3)vt.Normalized();
    
    auto vb = rows[3] + rows[1];
    vb /= vb.W;
    Bottom = (Vector3)vb.Normalized();
    
    auto vn = rows[3] - rows[2];
    vn /= vn.W;
    Near = (Vector3)vn.Normalized();
    
    auto vf = rows[3] + rows[2];
    vf /= vf.W;
    Far =  (Vector3)vf.Normalized();
    
}

bool Camera::ContainsSphere(const Vector3& center, const float radius, Vector3& containment)
{
    float dl = Left.Dot(center);
    float dr = Right.Dot(center);
    float dt = Top.Dot(center);
    float db = Bottom.Dot(center);
    float dn = Near.Dot(center);
    float df = Far.Dot(center);
    
    
    bool containsX = dl >= -radius && dr >= -radius;
    bool containsZ = 1;//dn >= -radius && df >= -radius;
    bool containsY = dt >= -radius && db >= -radius;
    
    bool containsAll = containsX && containsY && containsZ;
    
    containment.X = containsX;
    containment.Y = containsY;
    containment.Z = containsZ;
    
    return containsAll;
}


bool Camera::ContainsSphere(const Vector3& center, float radius)
{
    Vector4 containsBounds;
    
    return ContainsSphere(center, radius, containsBounds);
}

bool Camera::ContainsPoint(const Vector3& point)
{
    float dl = Left.Dot(point);
    float dr = Right.Dot(point);
    float dt = Top.Dot(point);
    float db = Bottom.Dot(point);
    
    auto contains = (dl > 0 && dr > 0 && db > 0 && dt > 0);
    
    Log::Info << "Contains " << contains << " l " << (dl > 0) << " r " << (dr > 0) << " t " << (dt  > 0) << " b " << (db > 0) << endl;
    
    
    return contains;
}




