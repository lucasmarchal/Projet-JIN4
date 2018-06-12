//
//  Block.cpp
//  Projet JIN4
//
//  Created by Lucas on 06/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#include "Block.hpp"
Block::Block(float x_position, float y_position, float width, float height, int value, sf::Color color)
: rectangle(sf::Vector2f(width, height)), value(value) {
    rectangle.setPosition(x_position, y_position);
    rectangle.setFillColor(color);
}
void Block::draw(sf::RenderWindow &window) const { window.draw(rectangle); }
const char Block::collision(sf::FloatRect &boundingBox) const {
    sf::FloatRect bounds = rectangle.getGlobalBounds();
    sf::Vector2f center(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
    float radius = boundingBox.width/2; // the object is a ball, so radius≈width(boundingBox)/2
    if (bounds.intersects(boundingBox)) {
        // if the top or bottom middle point of the ball is in the block rectangle,
        // it's a vertical collision
        if (bounds.contains(center.x, boundingBox.top) || bounds.contains(center.x, boundingBox.top+boundingBox.height)) {
            return 'v';
        }
        // if the left or right middle point of the ball is in the block rectangle, it's a horizontal collision
        else if (bounds.contains(boundingBox.left, center.y) || bounds.contains(boundingBox.left + boundingBox.width, center.y)) {
            return 'h';
        }
        // otherwise, we save the contact point
        // top left corner of the block
        else if (pow(pow(center.x-bounds.left, 2)+pow(center.y-bounds.top, 2), .5) < radius) {
            boundingBox.left = bounds.left;
            boundingBox.top = bounds.top;
            return 'c';
        }
        // top left corner of the block
        else if (pow(pow(center.x - (bounds.left+bounds.width), 2)+pow(center.y-bounds.top, 2), .5) < radius) {
            boundingBox.left = bounds.left + bounds.width;
            boundingBox.top = bounds.top;
            return 'c';
        }
        // bottom right corner of the block
        else if (pow(pow(center.x-(bounds.left+bounds.width), 2)+pow(center.y-(bounds.top+bounds.height), 2),.5) < radius) {
            boundingBox.left = bounds.left + bounds.width;
            boundingBox.top = bounds.top + bounds.height;
            return 'c';
        }
        // bottom left corner of the block
        else if (pow(pow(center.x - bounds.left, 2) + pow(center.y - (bounds.top + bounds.height), 2), .5) < radius) {
            boundingBox.left = bounds.left;
            boundingBox.top = bounds.top + bounds.height;
            return 'c';
        }
        // otherwise, the boxes intersect but there is no actual collision, nothing
        // 2B done
    }
    return 'n';
}
int Block::getValue() { return value; };
void Block::decrementValue() { value--; }
