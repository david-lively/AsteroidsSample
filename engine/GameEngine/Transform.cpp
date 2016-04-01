//
//  Transform.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Transform.h"
#include "Game.h"
#include "Bounds.h"

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
	Rotation = Vector3(0);

	Stop();
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


Matrix Transform::GetMatrix()
{
	auto mt = Matrix::CreateTranslation(Translation);
	auto mr = Matrix::CreateRotation(Rotation);
	auto ms = Matrix::CreateScale(Scale);

	auto lastMatrix = ms * mr * mt;

	return lastMatrix;
}


/// transformed Z axis
Vector3 Transform::Forward()
{
	auto m = GetMatrix();


	return Vector3(m.m20, m.m21, m.m22);
}

void Transform::Push(const Vector3& dir)
{
	Translation += dir;
}

void Transform::Push(const float x, const float y, const float z)
{
	Push(Vector3(x, y, z));
}

void Transform::Move(const float x, const float y, const float z)
{
	Move(Vector3(x, y, z));
}

/// move to a new position, but do not add velocity.
void Transform::Move(const Vector3& newPosition)
{
	auto velocity = Translation - m_previousTranslation;
	Translation = newPosition;
	m_previousTranslation = newPosition - velocity;
}

void Transform::Spin(const Vector3& theta)
{
	Rotation += theta;
}

void Transform::Spin(const  float x, const float y, const float z)
{
	Rotation += Vector3(x, y, z);
}

void Transform::Bounce(const Vector3& dir)
{
	Vector3 velocity = Translation - m_previousTranslation;
	Translation += dir * velocity.Length();
}


void Transform::OnUpdate(const GameTime& time)
{
	float timeScale = 1.f;

	//if (Game::Instance().Time.FrameNumber() > 10)
	//{
	//	timeScale = time.ElapsedSeconds() / m_previousFrameTime;
	//}

	m_previousFrameTime = time.ElapsedSeconds();

	auto velocity = (Translation - m_previousTranslation) * (1 - TranslationDrag) * timeScale;

	m_previousTranslation = Translation;
	Translation += velocity;

	auto spin = (Rotation - m_previousRotation) * (1 - TranslationDrag) * timeScale;
	m_previousRotation = Rotation;
	Rotation += spin;

	if (Scale.X != Scale.Y || Scale.Y != Scale.Z || Scale.Z != Scale.X)
		Log::Error << "Nonuniform scale detected: " << Scale << "\n";
}

BoundingSphere Transform::TransformSphere(const BoundingSphere& bounds)
{
	auto matrix = GetMatrix();

	Vector3 c1 = matrix.Transform(bounds.Center);

	Vector3 c2 = bounds.Center + Vector3(bounds.Radius, 0, 0);

	c2 = matrix.Transform(c2);

	float r1 = (c2 - c1).Length();


	return BoundingSphere(c1, r1);
}








