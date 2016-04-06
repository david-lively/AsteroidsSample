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
#include <queue>

#include "Game.h"
#include "Ship.h"
#include "WorldEntity.h"
#include "Asteroid.h"
#include "Grid.h"
#include "Light.h"
#include "Missile.h"
#include "Scoreboard.h"


class AsteroidsGame : public Game
{
public:
	AsteroidsGame() :  ShaderFolder("Shaders/")
    {
		Name = "ASTEROIDS";
    }
    
    bool OnCreateScene() override;
    
	void OnPreUpdate(const GameTime& time) override;
    void OnUpdate(const GameTime& time) override;
	
    /// location of shaders in the file system.
	std::string ShaderFolder;
    
    Ship& CreateShip();
    Grid& CreateGrid();
	void CreateLights(std::vector<Light*>& lights);

	void CreateAsteroids(const int count, const int total, std::vector<WorldEntity*>& entities);
	void DoCollisionCheck(const GameTime& time);
	void DoWrapping(const GameTime& time);

	bool CanRespawn();

	std::vector<std::tuple<WorldEntity*, WorldEntity*>> GetCollisionPairs();


private:
    Grid* m_grid = nullptr;
    Ship* m_ship = nullptr;
	Scoreboard* m_scoreboard = nullptr;
	

	std::queue<Drawable*> m_activeMissiles;
	std::queue<Drawable*> m_inactiveMissiles;

	/// entities that need to wrap when leaving the game area (frustum, grid or whatever)
	std::vector<WorldEntity*> m_itemsToWrap;
	std::vector<Light*> m_lights;

	std::vector<Asteroid*> m_asteroids;

	void Fire(Ship& ship);
	Drawable& GetAMissile();

};


#endif /* SimpleGame_hpp */
