//
//  GameObject.h
//
//  Created by David Lively on 2/1/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Common.h"
#include "Matrix.h"
#include "Identity.h"
//#include "Component.h"

class GameObject;

typedef std::function<void(const GameTime&, GameObject&)> GameEvent;

class GameObject : public Identity
{
public:
	bool Break = false;
    bool Enabled = true;
	GameObject* Parent = nullptr;

	int ReferenceCount = 0;
    
    GameObject() : GameObject("unnamed-gameobject")
    {
        
    }
    
    GameObject(const std::string& name) : Identity(name)
    {
		++ReferenceCount;
    }

	int CountObjects();

	void GetHierarchyParameters(int& objectCount, int& maxDepth);

	void BuildCombinedMatrix(Matrix& target);

    virtual void OnPreUpdate(const GameTime& time) {}
    virtual void OnUpdate(const GameTime& time) {}
    virtual void OnPostUpdate(const GameTime& time) {}
    
    virtual void OnPreRender(const GameTime& time) {}
    virtual void OnRender(const GameTime& time) {}
    virtual void OnPostRender(const GameTime& time) {}
    
    void Render(const GameTime& time);
    void Update(const GameTime& time);
    
    virtual void OnDispose() {}
    
    bool Initialize();
    virtual bool OnInitialize() { return true; }

	bool PostInitialize();
	virtual bool OnPostInitialize() { return true; }
    

	void WindowResize(int width, int height);
	virtual void OnWindowResize(int width, int height);


	template<typename T>
	bool Is()  
	{
		const T* ptr = dynamic_cast<T*>(this);
		return nullptr != ptr;
	}

	template<typename T>
	T& As()
	{
		T* ptr = dynamic_cast<T*>(this);

		if (nullptr == ptr)
			throw;

		return *ptr;
	}

	template<typename T>
	T* GetFirst() const
	{
		for (auto it = begin(m_children); it != end(m_children); ++it)
		{
			T* ptr = dynamic_cast<T*>(*it);

			if (nullptr != ptr)
				return ptr;
		}

		return nullptr;

	}
    
    /// create a new game object of the given type and add it as a child of this object.
    template<typename T>
    T& Create(const std::string& name)
    {
		std::shared_ptr<T> objectPtr = std::make_shared<T>(name);

		m_newObjects.push_back(objectPtr);
		
		objectPtr->Parent = this;

		return *objectPtr;
    }

	template<typename T>
	T& Add(std::shared_ptr<T> ptr)
	{
		ptr->Parent = this;
		m_children.push_back(ptr);
	}

	bool IsInitialized() const
	{
		return m_isInitialized;
	}

protected:
	void Dispatch(const GameTime& time, const std::vector<GameEvent>& events)
	{
		for (auto event : events)
			event(time, *this);

	}

	void Dispatch(const GameTime& time)
	{

	}

    
private:
	bool m_isInitialized = false;
	std::vector<std::shared_ptr<GameObject>> m_children;

    void DoPreRender(const GameTime& time);
    void DoPostRender(const GameTime& time);
    
    void DoPreUpdate(const GameTime& time);
    void DoPostUpdate(const GameTime& time);
    
    void DoRender(const GameTime& time);
    void DoUpdate(const GameTime& time);
    

	// objects created this frame to be moved to m_children at the start of the next frame
	std::vector<std::shared_ptr<GameObject>> m_newObjects;

	//std::vector<std::shared_ptr<Component>> m_components;
    
protected:
	void ProcessNewObjects();

    
};


#endif /* GameObject_hpp */
























