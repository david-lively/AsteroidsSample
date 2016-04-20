//
//  Game.cpp
//
//  Created by David Lively on 2/1/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include <iostream>

#include "Game.h"
#include "InputHandler.h"
#include "Shader.h"


Game* Game::m_instance = nullptr;
Camera* Game::m_camera = nullptr;
using namespace std;


void framebufferSizeChanged(GLFWwindow* window, int width, int height)
{
	gl::Viewport(0, 0, width, height);
	Game::Instance().WindowResize(width, height);
}

void errorCallback(int code, const char* message)
{
	Log::Error << "GLFW Error: (" << code << ") \"" << message << "\"" << endl;
}



Game::Game() :
m_window(nullptr)
, ClearColor(0, 0, 0, 1)
{
	if (nullptr != m_instance)
	{
		Log::Error << "Only one instance of Game is allowed\n";
		throw;
	}

	m_instance = this;

	srand((unsigned int)time(NULL));
}


bool Game::OnInitialize()
{
	/* Initialize the library */
	if (!glfwInit())
	{
		Log::Error << "Could not initialize GLFW\n";
		return false;
	}

	/// specify some window and OpenGL API parameters
	/// enable multisampling on a 4x4 grid (for full-screen anti-aliasing)
	glfwWindowHint(GLFW_SAMPLES, 4);

	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(1280 * 2, 720 * 2, "Asteroids", NULL, NULL);

	if (!m_window)
	{
		Log::Error << "Could not create GLFW window\n";
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);

	if (!gl::sys::LoadFunctions())
	{
		Log::Error << "Could not bind OpenGL function pointers.\n";
		glfwTerminate();
		return false;
	}


	check_gl_error();

	const GLubyte* renderer = gl::GetString(gl::RENDERER); // get renderer string
	const GLubyte* version = gl::GetString(gl::VERSION); // version as a string

	Log::Info << "Renderer " << renderer << endl;
	Log::Info << "OpenGL version supported: " << version << endl;


	glfwSetFramebufferSizeCallback(m_window, framebufferSizeChanged);
	glfwSetErrorCallback(errorCallback);

	int w, h;

	glfwGetFramebufferSize(m_window, &w, &h);
	WindowResize(w, h);

	if (nullptr == m_camera)
		m_camera = &Create<class Camera>("primary.camera");

	if (nullptr == m_environment)
		m_environment = &Create<GameEnvironment>("game.environment");

	if (!OnCreateScene())
		return false;

	gl::ClearColor(ClearColor.X, ClearColor.Y, ClearColor.Z, ClearColor.W);


	return true;
}

bool Game::Run()
{
	if (!Initialize())
	{
		Log::Error << "Could not initialize GLFW window.\n";
		return false;
	}

	IsPlaying = true;

	int prevPauseKeyState = false;

	Log::Debug << "Starting main update/render loop." << endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		ProcessNewObjects();

		/* Poll for and process events */
		glfwPollEvents();

		int pauseKeyState = glfwGetKey(m_window, GLFW_KEY_P);

		if (prevPauseKeyState && !pauseKeyState)
		{
			IsPlaying = !IsPlaying;
		}

		prevPauseKeyState = pauseKeyState;
		if (IsPlaying)
			Update(Time);

		gl::ClearColor(ClearColor.X, ClearColor.Y, ClearColor.Z, ClearColor.W);
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

		Render(Time);

		/* Render here */
		/* Swap front and back buffers */
		glfwSwapBuffers(m_window);

		if (IsPlaying)
			Time.Update();
	}

	return true;

}




void Game::Shutdown()
{
	if (nullptr != m_window)
	{
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}

}

/// <summary>
/// returns the size of the framebuffer (the thing we're drawing to)
/// </summary>
void Game::GetFramebufferSize(int* width, int* height)
{
	if (!m_instance->m_window)
	{
		Log::Error << "No GLFW window is available. Create a window before calling Game::GetFrameBufferSize\n";
		DEBUG_BREAK;
	}

	glfwGetFramebufferSize(m_instance->m_window, width, height);


}











