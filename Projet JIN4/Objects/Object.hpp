//
//  Object.hpp
//  Projet JIN4
//
//  Created by Lucas on 05/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include "link.hpp"
class Object {
public:
    virtual ~Object() = default;
    virtual const char collision(sf::FloatRect& boundingBox) const = 0; //Determines if another object collides with this one and returns the collision type
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual void move() {};
    virtual void decrementValue()=0;
    virtual int getValue()=0;
    bool remove = false; //delete before the next frames
};
#endif /* Object_hpp */
