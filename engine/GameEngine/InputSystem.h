#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <map>
#include <vector>
#include <functional>
#include <queue>

#include "Common.h"
#include "GameObject.h"

typedef std::function<void(const GameTime&)> KeyHandler;

struct KeyEvent
{
	int Key;
	int Modifiers;
	int Action;
	
	bool operator==(const KeyEvent& rval)
	{
		return Key == rval.Key
			&& Modifiers == rval.Modifiers
			&& Action == rval.Action
			;
	}

	bool operator<(const KeyEvent& rval) const
	{
		return Key < rval.Key || Modifiers < rval.Modifiers || Action < rval.Action;
	}

};


class InputSystem
{
public:
	static void Subscribe(int key, int modifiers, int action, KeyHandler handler);

	static void Dispatch();

	static InputSystem& Instance();
private:

#endif



//
//
///*! @brief The function signature for keyboard key callbacks.
//*
//*  This is the function signature for keyboard key callback functions.
//*
//*  @param[in] window The window that received the event.
//*  @param[in] key The [keyboard key](@ref keys) that was pressed or released.
//*  @param[in] scancode The system-specific scancode of the key.
//*  @param[in] action @ref GLFW_PRESS, @ref GLFW_RELEASE or @ref GLFW_REPEAT.
//*  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
//*  held down.
//*
//*  @sa glfwSetKeyCallback
//*
//*  @ingroup input
//*/
////typedef void(*GLFWkeyfun)(GLFWwindow*, int, int, int, int);
//void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
//{
//
//}
