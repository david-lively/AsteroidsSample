//
//  InputHandler.hpp
//  GameEngine
//
//  Created by David Lively on 2/25/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Common.h"
#include "GameObject.h"

#include <map>
#include <vector>
#include <functional>

/// shortcut for declaring input handlers
#define DECL_KEYHANDLER [=](const GameObject& sender, const GameTime& time)

typedef std::function<void(const GameObject&, const GameTime&)> KeyHandler;


enum class KeyState
{
    Non
    ,Press
    ,Hold
    ,Release    
};


class InputHandler : public GameObject
{
public:
	InputHandler() : InputHandler("inputHandler")
	{
	}

	InputHandler(const std::string& name) : GameObject(name)
	{

	}


    void OnPostUpdate(const GameTime& time) override;
    void Subscribe(int keyCode, KeyHandler handler);
    
private:


    std::map<int, std::vector<KeyHandler>> m_keyHandlers;
    /// previous key states from GLFW 
    std::map<int,KeyState>  m_keyStates;
    
    void UpdateKeyStates();
};


#endif /* InputHandler_hpp */
