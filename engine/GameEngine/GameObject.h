//
//  GameObject.hpp
//  Assignment2
//
//  Created by David Lively on 2/1/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>
#include <map>

#include "Common.h"
#include "Matrix.h"

class GameObject
{
public:
	bool Break = false;
    std::string Name;
    int Id;
    bool Enabled = true;
	GameObject* Parent = nullptr;
    
    GameObject() : GameObject("unnamed-gameobject")
    {
        
    }
    
    GameObject(const std::string& name) : Name(name)
    {
        Id = ++m_nextId;
    }


	void PrintHierarchy(int indent);
    
	void BuildCombinedMatrix(Matrix& target);
	void Add(const GameObject& item);
	void SetParent(GameObject* newParent);


    virtual void OnPreUpdate(const GameTime& time) {}
    virtual void OnUpdate(const GameTime& time) {}
    virtual void OnPostUpdate(const GameTime& time) {}
    
    virtual void OnPreRender(const GameTime& time) {}
    virtual void OnRender(const GameTime& time) {}
    virtual void OnPostRender(const GameTime& time) {}
    
    virtual void Render(const GameTime& time);
    virtual void Update(const GameTime& time);
    
    virtual void OnDispose() {}
    
    bool Initialize();
    virtual bool OnInitialize() { return true; }
    

	void WindowResize(int width, int height);
	virtual void OnWindowResize(int width, int height);

    void Dispose();

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
        auto* object = new T();
        object->Name = name + std::to_string(object->Id);

		m_newObjects.push_back(object);
        
		object->Parent = this;
        return *object;
    }
    
    /// Find an object of type T in the current hierarchy. If not found, create a new one.
    //template<typename T>
    //T& GetObject(const std::string& name)
    //{
    //    auto* objectPtr = FindObject(name);
    //    
    //    if (nullptr == objectPtr)
    //    {
    //        T* ptr = new T();
    //        ptr->Name = name;
    //        
    //        m_children.push_back(ptr);
    //        
    //        return ptr;
    //    }
    //    else
    //        return *objectPtr;
    //    
    //}

    
    /// If an object with the given name exists in this hierarchy, return a pointer to it.
    //void* FindObject(const std::string& name)
    //{
    //    if (name == Name)
    //        return this;
    //    
    //    for(auto it = begin(m_children); it != end(m_children); ++it)
    //    {
    //        auto* item = (*it)->FindObject(name);
    //        if (nullptr != item)
    //            return item;
    //    }
    //    
    //    return nullptr;
    //}
    
private:
	bool m_isInitialized = false;
    std::vector<GameObject*> m_children;

    void DoPreRender(const GameTime& time);
    void DoPostRender(const GameTime& time);
    
    void DoPreUpdate(const GameTime& time);
    void DoPostUpdate(const GameTime& time);
    
    void DoRender(const GameTime& time);
    void DoUpdate(const GameTime& time);
    
    static int m_nextId;

	// objects created this frame to be moved to m_children at the start of the next frame
	std::vector<GameObject*> m_newObjects;


    
    
protected:
	void ProcessNewObjects();

    
};


#endif /* GameObject_hpp */
























