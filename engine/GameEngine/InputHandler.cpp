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

void InputHandler::OnPostUpdate(const GameTime& time)
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
                (*func)(time);
            }
            
        }
        
    }

	GameObject::OnPostUpdate(time);
}


void InputHandler::UpdateKeyStates()
{
    //for(auto it = begin(m_keyStates); it != end(m_keyStates); ++it)
    //{
    //}
}

void InputHandler::Subscribe(int keyCode, KeyHandler handler, int action, int modifiers)
{
	//InputSystem::Subscribe(keyCode, modifiers, action, handler);
}







