//
//  Block.hpp
//  Projet JIN4
//
//  Created by Lucas on 06/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp

#include "Object.hpp"
#include "Game.hpp"
class Block : public Object {
public:
    Block(float x_position, float y_position, float width, float height, int value, sf::Color color = BLOCK_COLOR);
    void draw(sf::RenderWindow& window) const;
    const char collision(sf::FloatRect& boundingBox) const;
    int getValue();
    void decrementValue();
private:
    sf::RectangleShape rectangle;
    int value;
};
#endif /* Block_hpp */
