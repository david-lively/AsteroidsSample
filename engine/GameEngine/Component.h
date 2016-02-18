//
//  Component.hpp
//  GameEngine
//
//  Created by David Lively on 2/18/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef COMPONENT_H
#define COMPONENT_H

#include "Common.h"

#include <string>

/*
 Base class for all components
*/
class Component
{
public:
    /// application-specific data
    std::string Tag;
    
    Component() {}

    /// pure virtual destructor - make this an abstract class
    virtual ~Component() = 0;
    
    virtual void Initialize() {}
    virtual void Dispose() {}
private:
};

#endif /* Component_hpp */
