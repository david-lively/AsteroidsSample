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

class AsteroidsGame : public Game
{
public:
	AsteroidsGame() : ShaderFolder("Shaders/")
    {
    }
    
    bool OnCreateScene() override;
    
    void OnUpdate(const GameTime& time) override;
    
    /// location of shaders in the file system.
	std::string ShaderFolder;
    
    Ship& CreateShip();
    Grid& CreateGrid();
    
	void CreateAsteroids(int count, std::vector<WorldEntity*>& entities);

    
private:
    Grid* m_grid;
    Ship* m_ship;
	/// entities that need to wrap when leaving the game area (frustum, grid or whatever)
	std::vector<WorldEntity*> m_itemsToWrap;
    
};


#endif /* SimpleGame_hpp */
