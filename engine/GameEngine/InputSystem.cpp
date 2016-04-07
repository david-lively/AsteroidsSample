#include "InputSystem.h"
#include "Common.h"
#include "Game.h"

#include <vector>
#include <map>
#include <functional>
#include <queue>

using namespace std;

InputSystem* InputSystem::m_instance = InputSystem();

/*
events[key][modifiers][action] = handler
*/
std::map<KeyEvent, vector<KeyHandler>> keyEvents;

std::queue<KeyHandler> pendingEvents;

void keyCallback(GLFWwindow* window, int key, int modifiers, int scancode, int action)
{
	Log::Info << "Keyboard: key " << key << " action " << modifiers << " action " << action << "\r";

	KeyEvent e;
	
	e.Key = key;
	e.Modifiers = modifiers;
	e.Action = action;

	if (keyEvents.count(e) > 0)
	{
		for (auto handler : keyEvents[e])
		{
			pendingEvents.push(handler);
		}
	}
}


InputSystem::InputSystem()
{
	if (nullptr != m_instance)
	{
		Log::Error << "Only one instance of InputSystem is allowed." << endl;
	}

	m_instance = this;
	glfwSetKeyCallback(Game::Instance().Window(), keyCallback);
}


InputSystem::~InputSystem()
{
}

void InputSystem::Subscribe(int key, int modifiers, int action, KeyHandler handler)
{
	KeyEvent e;

	e.Key = key;
	e.Modifiers = modifiers;
	e.Action = action;

	keyEvents[e].push_back(handler);
}


void InputSystem::Dispatch()
{
}


