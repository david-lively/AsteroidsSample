//
//  SimpleGame.hpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include <vector>

#include "Game.h"
#include "Ship.h"
#include "WorldEntity.h"
#include "Asteroid.h"
#include "Grid.h"
#include "Light.h"


class AsteroidsGame : public Game
{
public:
	AsteroidsGame() : ShaderFolder("Shaders/")
    {
    }
    
    bool OnCreateScene() override;
    
	void OnPreUpdate(const GameTime& time) override;
    void OnUpdate(const GameTime& time) override;
	
    /// location of shaders in the file system.
	std::string ShaderFolder;
    
    Ship& CreateShip();
    Grid& CreateGrid();
	void CreateLights(std::vector<Light*>& lights);

	void CreateAsteroids(int count, std::vector<WorldEntity*>& entities);
    
private:
    Grid* m_grid;
    Ship* m_ship;
	/// entities that need to wrap when leaving the game area (frustum, grid or whatever)
	std::vector<WorldEntity*> m_itemsToWrap;
	std::vector<Light*> m_lights;

	std::vector<Asteroid*> m_asteroids;


};


#endif /* SimpleGame_hpp */
