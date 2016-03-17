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

Matrix Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

void Camera::OnPreUpdate(const GameTime& time)
{
	auto t = Transform->Translation * -1.f;
	auto r = Transform->Rotation * -1.f;

	m_viewMatrix = Matrix::CreateRotation(r) * Matrix::CreateTranslation(t);
}

Matrix Camera::GetViewMatrix()
{
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
	int w, h;

	Game::GetFramebufferSize(&w, &h);
	const float aspect = w * 1.f / h;

	const float halfHeight = ZNear * tan(FieldOfView / 2.f);
	const float halfWidth = halfHeight * aspect;

	/// corners of the viewport
	Vector3 tr(halfWidth, halfHeight, ZNear);
	Vector3 br(halfWidth, -halfHeight, ZNear);
	Vector3 bl(-halfWidth, -halfHeight, ZNear);
	Vector3 tl(-halfWidth, halfHeight, ZNear);

	/// frustum plane normals
	Top = Vector3::Cross(tl, tr).Normalized();
	Right = Vector3::Cross(tr, br).Normalized();
	Bottom = Vector3::Cross(br, bl).Normalized();
	Left = Vector3::Cross(bl, tl).Normalized();

}

bool Camera::ContainsSphere(const Vector3& center, const float radius, Vector3& containment)
{
	float dt = center.Dot(Top);
	float dr = center.Dot(Right);
	float db = center.Dot(Bottom);
	float dl = center.Dot(Left);

	float dn = center.Z - ZNear;
	float df = ZFar - center.Z;

	float r = radius;
	bool contains = dl > -r && dr > -r && dt > -r && db > -r;

	containment.X = dl > -r && dr > -r;
	containment.Y = dt > -r && db > -r;
	containment.Z = 1;// dn > -r && db > -r;

	return contains;
}


bool Camera::ContainsSphere(const Vector3& center, const float radius)
{
	float dt = center.Dot(Top);
	float dr = center.Dot(Right);
	float db = center.Dot(Bottom);
	float dl = center.Dot(Left);

	float dn = center.Z - ZNear;
	float df = ZFar - center.Z;

	float scale = 0.2f;
	dl *= scale;
	dr *= scale;
	dt *= scale;
	db *= scale;

	float r = radius;
	bool contains = dl > -r && dr > -r && dt > -r && db > -r;

	return contains;

}

//bool Camera::ContainsSphere(const Vector3& center, float radius)
//{
//	Vector4 containsBounds;
//
//	return ContainsSphere(center, radius, containsBounds);
//}

bool Camera::ContainsPoint(const Vector3& point)
{
	float dl = Left.Dot(point);
	float dr = Right.Dot(point);
	float dt = Top.Dot(point);
	float db = Bottom.Dot(point);

	auto contains = (dl > 0 && dr > 0 && db > 0 && dt > 0);

	Log::Info << "Contains " << contains << " l " << (dl > 0) << " r " << (dr > 0) << " t " << (dt > 0) << " b " << (db > 0) << endl;


	return contains;
}


/*
Update projection matrix when the window size changes.
*/
void Camera::OnWindowResize(int width, int height)
{
	UpdateProjectionMatrix(width, height);
}

bool Camera::OnInitialize()
{
	/// calculate initial projection matrix
	int w, h;
	GLFWwindow* window = Game::Instance().Window();

	glfwGetWindowSize(window, &w, &h);


	UpdateProjectionMatrix(w, h);

	return true;
}

void Camera::SetFieldOfView(float newFov)
{ 
	FieldOfView = newFov;

	UpdateProjectionMatrix();
}

void Camera::UpdateProjectionMatrix(int w, int h)
{
	if (0 == w || 0 == h)
	{
		Game::GetFramebufferSize(&w, &h);
	}

	float aspect = w * 1.f / h;

	m_projectionMatrix = Matrix::CreatePerspective(FieldOfView, aspect, ZNear, ZFar);

	BuildFrustumPlanes();
}








