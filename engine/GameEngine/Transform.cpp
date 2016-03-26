//
//  Transform.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Transform.h"
#include "Game.h"


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


Matrix Transform::GetMatrix()
{
	//static int lastFrame = 0;
	//static Matrix lastMatrix;

	//auto& time = Game::Instance().Time;

	//if (time.FrameNumber() != lastFrame)
	//{
	//	lastFrame = time.FrameNumber();
	
		auto mt = Matrix::CreateTranslation(Translation);
		auto mr = Matrix::CreateRotation(Rotation);
		auto ms = Matrix::CreateScale(Scale);

		auto lastMatrix = ms * mr * mt;
	//}

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
	auto velocity = Translation - m_previousTranslation;
	Translation = Vector3(x, y, z);
	m_previousTranslation = Translation - velocity;
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

void Transform::OnUpdate(const GameTime& time) 
{
	//float timeScale = m_prev time.ElapsedSeconds() / m_previousFrameTime;
	float timeScale = 1;
	auto velocity = (Translation - m_previousTranslation) * (1 - Drag) * timeScale;

	m_previousTranslation = Translation;
	Translation += velocity;

	auto spin = (Rotation - m_previousRotation) * (1 - Drag);
	m_previousRotation = Rotation;
	Rotation += spin;

	if (Scale.X != Scale.Y || Scale.Y != Scale.Z || Scale.Z != Scale.X)
		Log::Error << "Nonuniform scale detected: " << Scale << "\n";
}







