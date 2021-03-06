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
#include <set>

#include "Game.h"
#include "Ship.h"
#include "WorldEntity.h"
#include "Asteroid.h"
#include "Grid.h"
#include "Light.h"
#include "Missile.h"
#include "Scoreboard.h"
#include "Sprite.h"
#include "StateMachine.h"
#include "Hud.h"
#include "Text.h"

class AsteroidsGame : public Game
{
public:
	AsteroidsGame() : ShaderFolder("")
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

	void CreateAsteroids(const int count, const int level = 0);
	void DoCollisionCheck(const GameTime& time);
	void DoWrapping(const GameTime& time);

	bool CanRespawn();

	std::vector<std::tuple<WorldEntity*, WorldEntity*>> GetCollisionPairs();
	void OnReset(const GameTime& time) override;

	WorldEntity& CreateMap();


private:
	StateMachine* m_stateMachine;

	Hud* m_hud = nullptr; 
    Grid* m_grid = nullptr;
    Ship* m_ship = nullptr;
	Drawable* m_map;

	Scoreboard* m_scoreboard = nullptr;
	

	std::vector<Drawable*> m_allMissiles;

	std::queue<Drawable*> m_inactiveMissiles;
	std::queue<Asteroid*> m_inactiveAsteroids;
	/// entities that need to wrap when leaving the game area (frustum, grid or whatever)
	std::vector<WorldEntity*> m_itemsToWrap;
	std::vector<Light*> m_lights;

	std::vector<Asteroid*> m_asteroids;

	void Fire(Ship& ship);

	void DisableMissile(Drawable& missile);
	void UpdateStatus();
	Drawable& GetMissile(bool forceCreateNew = false);
	
	Asteroid& CreateAsteroid();
	Asteroid& GetAsteroid(bool forceCreateNew = false);

	void ExpandMissilePool(const int count);

	StateMachine& CreateStateMachine();

	void CreateTextPool();
};


#endif /* SimpleGame_hpp */
