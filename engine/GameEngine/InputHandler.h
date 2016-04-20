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
//#define DECL_KEYHANDLER [=](const GameObject& sender, const GameTime& time)
#define DECL_KEYHANDLER [=](const GameTime& time)
typedef std::function<void(const GameTime&)> KeyHandler;




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
	void Subscribe(int key, KeyHandler handler);
    
private:
    std::map<int, std::vector<KeyHandler>> m_keyHandlers;
    
    void UpdateKeyStates();
};


#endif /* InputHandler_hpp */
