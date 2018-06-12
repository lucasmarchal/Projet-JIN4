//
//  Frame.cpp
//  Projet JIN4
//
//  Created by Lucas on 05/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#include "Frame.hpp"
Frame::Frame(float windowWidth, float windowHeight, float width, float buffer, sf::Color color)
:
left(sf::Vector2f(width, windowHeight - buffer - TOOLBAR_HEIGHT)),
top(sf::Vector2f(windowWidth - 2 * buffer - width, width)), // 2 buffers 4 extra space on left & left
right(sf::Vector2f(width, windowHeight - buffer - TOOLBAR_HEIGHT))
{
    left.setPosition(buffer, buffer + TOOLBAR_HEIGHT);
    top.setPosition(buffer, buffer + TOOLBAR_HEIGHT);
    right.setPosition(windowWidth - buffer - width, buffer + TOOLBAR_HEIGHT);
    left.setFillColor(color);
    top.setFillColor(color);
    right.setFillColor(color);
}
void Frame::draw(sf::RenderWindow &window) const {
    window.draw(left);
    window.draw(top);
    window.draw(right);
}
const char Frame::collision(sf::FloatRect &boundingBox) const {
    if (left.getGlobalBounds().intersects(boundingBox) ||
        right.getGlobalBounds().intersects(boundingBox)) {
        return 'h'; // horizontal
    } else if (top.getGlobalBounds().intersects(boundingBox)) {
        return 'v'; // vertical
    } else {
        return 'n'; // none
    }
}
int Frame::getValue(){return 1;};
void Frame::decrementValue(){};
