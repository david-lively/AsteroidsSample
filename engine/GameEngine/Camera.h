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
    
    Camera() : FieldOfView(TO_RADIANS(60)), ZNear(1), ZFar(1000)
    {
    }

	void OnWindowResize(int w, int h) override;
	bool OnInitialize() override;

	void OnPreUpdate(const GameTime& time);

	void SetFieldOfView(float newFov);

    Matrix GetProjectionMatrix();
    Matrix GetViewMatrix();
    Matrix GetViewProjectionMatrix();
    
    bool ContainsSphere(const Vector3& center, const float radius);
    bool ContainsSphere(const Vector3& center, const float radius, Vector3& containment);
    bool ContainsPoint(const Vector3& point);
	void UpdateProjectionMatrix(int width = 1280, int height = 720);
private:

    Matrix m_projectionMatrix;
    Matrix m_viewMatrix;
    
    void BuildFrustumPlanes();
    
};


#endif /* Camera_h */
