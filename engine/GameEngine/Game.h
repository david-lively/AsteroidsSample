//
//  Game.hpp
//  Assignment2
//
//  Created by David Lively on 2/1/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <vector>

#include "Common.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameEnvironment.h"

class Game : public GameObject
{
public:
    GameTime Time;
	bool IsPlaying = false;
	Vector4 ClearColor;
	
	
	Game();
	
	static Camera& Camera()
    {
        if (nullptr == m_camera)
            m_camera = &m_instance->Create<class Camera>("primary.camera");
        
        return *m_camera;
    }
    
    bool Run();
    
    bool OnInitialize() override;

	GameEnvironment& Environment()
	{
		return m_environment;

	}
    
    virtual bool OnCreateScene()
    {
        Log::Info << "Create any scene objects here.\n";
        return true;
    }

	static void GetFramebufferSize(int* width, int* height);
    
    static Game& Instance() { return *m_instance; }
    
    GLFWwindow* Window() const { return m_window; }


private:
    GLFWwindow* m_window;
    static Game* m_instance;
    static class Camera* m_camera;
	GameEnvironment m_environment;
    
    bool m_isInitialized;

    void Shutdown();
    
};

#endif /* Game_hpp */
