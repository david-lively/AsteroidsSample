//
//  InputHandler.cpp
//  GameEngine
//
//  Created by David Lively on 2/25/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "InputHandler.h"
#include "Common.h"
#include "Game.h"

#include <vector>
#include <map>

using namespace std;

void InputHandler::OnUpdate(const GameTime& time)
{
    UpdateKeyStates();
    //    auto* window = Game::Instance().Window();
    //
    //    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
    //    {
    //        Log::Info << "ESCape key pressed. Closing.\n";
    //        glfwSetWindowShouldClose(window, true);
    //    }
    auto* window = Game::Instance().Window();
    
    for(auto it = begin(m_keyHandlers); it != end(m_keyHandlers); ++it)
    {
        if (GLFW_PRESS == glfwGetKey(window, it->first))
        {
            vector<KeyHandler>& funcs = it->second;
            
            for(auto func = begin(funcs); func != end(funcs); ++func)
            {
                (*func)(*this,time);
            }
            
        }
        
    }
    
    
}


void InputHandler::UpdateKeyStates()
{
    for(auto it = begin(m_keyStates); it != end(m_keyStates); ++it)
    {
    }
}

void InputHandler::Subscribe(int keyCode, KeyHandler handler)
{
    auto state = glfwGetKey(Game::Instance().Window(), keyCode);
    
    m_keyHandlers[keyCode].push_back(handler);
}





