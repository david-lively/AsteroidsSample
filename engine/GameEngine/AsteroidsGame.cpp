//
//  SimpleGame.cpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include <map>
#include <vector>
#include <vector>
#include <iostream>

using namespace std;

#include "AsteroidsGame.h"
#include "Mesh.h"
#include "Material.h"
#include "Files.h"
#include "InputHandler.h"
#include "Grid.h"
#include "Asteroid.h"

#include <cmath>



void QuitGame(const GameObject& sender, const GameTime& time)
{
    Log::Info << "Closing application\n";
    glfwSetWindowShouldClose(Game::Instance().Window(), true);
}

bool AsteroidsGame::OnCreateScene()
{
    m_ship = &CreateShip();
    m_grid = &CreateGrid();
    
    CreateAsteroids(1);

    auto& input = Create<InputHandler>("asteroids-input");
    
   
    input.Subscribe(GLFW_KEY_ESCAPE,
                    DECL_KEYHANDLER
                    {
                        Log::Info << "Quit key received, closing.\n";
                        glfwSetWindowShouldClose(Game::Instance().Window(), true);
                    }
                    
                    );
    
    input.Subscribe(GLFW_KEY_F1,
                    DECL_KEYHANDLER
                    {
                        if (nullptr != m_grid)
                            m_grid->Enabled = !m_grid->Enabled;
                    }
                    
                    );
    
    input.Subscribe(GLFW_KEY_W,
                    DECL_KEYHANDLER
                    {
                        auto& camera = Game::Camera();
                        
                        camera.Transform->Spin(Vector3(0.01f, 0, 0));
                    }
                    );

    input.Subscribe(GLFW_KEY_S,
                    DECL_KEYHANDLER
                    {
                        auto& camera = Game::Camera();
                        
                        camera.Transform->Spin(Vector3(-0.01f, 0, 0));
                    }
                    );
    

    input.Subscribe(GLFW_KEY_E,
                    DECL_KEYHANDLER
                    {
                        auto& camera = Game::Camera();
                        
                        camera.Transform->Reset();
                    }
                    );
    
    
    return true;
}


void AsteroidsGame::OnUpdate(const GameTime& time)
{
    static bool previousIntersects = true;
//    /// clip the ship to the grid.
//    auto& cam = Game::Camera();
//    
//    auto view = cam.GetViewMatrix();
//    
//    auto m = m_ship->Transform->GetMatrix();// * view;
//    
//    auto shipTranslation = m_ship->Transform->Translation;
//    
//    auto viewPos = m.Transform(Vector4(0,0,0,1));
//    
//    Log::Info << "ship " << viewPos << endl;
    
    auto shipBounds = m_ship->Transform->TransformAABB(m_ship->Bounds);
    auto gridBounds = m_grid->Transform->TransformAABB(m_grid->Bounds);
    
    auto intersects = shipBounds.Intersects(gridBounds) != IntersectionType::Disjoint;
    
    if(!intersects)
    {
        auto moveTo = m_ship->Transform->Translation;
        if (shipBounds.Max.X < gridBounds.Min.X || shipBounds.Min.X > gridBounds.Max.X)
        {
            moveTo.X = -1.f * moveTo.X;
        }

        if (shipBounds.Max.Y < gridBounds.Min.Y || shipBounds.Min.Y > gridBounds.Max.Y)
        {
            moveTo.Y = -1.f * moveTo.Y;
        }
        
        m_ship->Transform->Move(moveTo);
        
    }
    
    //Log::Info << "Ship::Grid intersection: " << intersects << " ship mn " << shipBounds.Min << " mx " << shipBounds.Max << " grid mn " << gridBounds.Min << " mx " << gridBounds.Max << endl;
    
    
}



Ship& AsteroidsGame::CreateShip()
{
    auto& ship = Create<Ship>("ship");

    ship.Transform->Scale = Vector3(1.5f);
    
    return ship;
}

Grid& AsteroidsGame::CreateGrid()
{
    auto& grid = Create<Grid>("grid");
    
    grid.Transform->Scale = Vector3(20);
    
    return grid;
    
}

void AsteroidsGame::CreateAsteroids(int count)
{
    Log::Warning << "Ignoring asteroid count (" << count << ") - creating 1 for debug\n";
    
    auto& asteroid = Create<Asteroid>("asteroid");
    asteroid.Transform->Scale = Vector3(2);
}




