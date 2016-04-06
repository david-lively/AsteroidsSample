//
//  Missile.h
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef MISSILE_H
#define MISSILE_H

#include "Common.h"
#include "Drawable.h"

class Missile : public Drawable
{
public:
	Missile(const std::string& name) : Drawable(name)
	{
		OnExitFrustum = FrustumAction::Recycle;

	}

	Missile() : Missile("missile")
    {
    }

	bool OnInitialize() override;

	void OnRender(const GameTime& time) override
	{
		Material.Bind();

		Vector4 orange(1, 165.f / 255.f, 0, 0.125f);

		Uniforms.SetUniform("EmissiveColorIntensity", orange);

		Drawable::OnRender(time);
	}

};


#endif /* Missile_h */

