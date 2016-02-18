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
#include "Component.h"
#include "Transform.h"

class GameObject
{
public:
    std::string Name;
    int Id;
    
    GameObject() : GameObject("unnamed-gameobject")
    {
        
        
    }
    
    GameObject(const std::string& name) : Name(name), m_parent(nullptr)
    {
        Id = ++m_nextId;
    }
    
    ~GameObject()
    {
        Dispose();
    }
    
    
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
    
    void Dispose();

    
    /// create a new game object of the given type and add it as a child of this object.
    template<typename T>
    T& Create(const std::string& name)
    {
        auto* object = new T();
        object->m_parent = this;
        object->Name = name;
        
        m_children.push_back(object);
        
        return *object;
    }
    
    /// Find an object of type T in the current hierarchy. If not found, create a new one.
    template<typename T>
    T& GetObject(const std::string& name)
    {
        auto* objectPtr = FindObject(name);
        
        if (nullptr == objectPtr)
        {
            T* ptr = new T();
            ptr->Name = name;
            ptr->m_parent = this;
            
            m_children.push_back(ptr);
            
            return ptr;
        }
        else
            return *objectPtr;
        
    }
    
    /// If an object with the given name exists in this hierarchy, return a pointer to it.
    void* FindObject(const std::string& name)
    {
        if (name == Name)
            return this;
        
        for(auto it = begin(m_children); it != end(m_children); ++it)
        {
            auto* item = (*it)->FindObject(name);
            if (nullptr != item)
                return item;
        }
        
        return nullptr;
    }
    
    GameObject& Parent() const
    {
        return *m_parent;
    }
    
private:
    static int m_nextId;
    GameObject* m_parent;

    std::vector<GameObject*> m_children;
    std::vector<Component*> m_components;

    void DoPreRender(const GameTime& time);
    void DoPostRender(const GameTime& time);
    
    void DoPreUpdate(const GameTime& time);
    void DoPostUpdate(const GameTime& time);
    
    void DoRender(const GameTime& time);
    void DoUpdate(const GameTime& time);
    
    void DisposeChildren();
    void DisposeComponents();
    
    
    
protected:
    
    
};


#endif /* GameObject_hpp */
























