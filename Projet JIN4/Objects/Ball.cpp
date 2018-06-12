//
//  Ball.cpp
//  Projet JIN4
//
//  Created by Lucas on 05/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#include "Ball.hpp"

Ball::Ball(Game &game, float x_position, float y_position, float x_velocity, float y_velocity, sf::Color color, float radius)
: game(game), circle(radius, 50), velocity(x_velocity, y_velocity),
attachedStatus(false) {
    circle.setOrigin(radius, radius);
    circle.setOutlineThickness(1);
    circle.setOutlineColor(BALL_COLOR);
    circle.setFillColor(color);
    circle.setPosition(x_position, y_position);
}
Ball::Ball(Game &game, sf::Color color, float radius)
: game(game), circle(radius, 50), velocity(0, 0), attachedStatus(true) {
    circle.setOrigin(radius, radius);
    circle.setOutlineThickness(1);
    circle.setOutlineColor(BALL_COLOR);
    circle.setFillColor(color);
    circle.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT);
}
void Ball::draw(sf::RenderWindow &window) const { window.draw(circle); }
void Ball::move() {
    sf::FloatRect bounds = circle.getGlobalBounds();
    if (bounds.top > game.windowSize.y + REMOVAL_MARGIN) { // mark for removal if the ball has left the screen
        remove = true;
        return;
    }
    std::vector<Object*>& objects = game.getObjects();
    if (attachedStatus || (frameCollision(bounds)) || (blockCollision(bounds, objects))) {
        // moves the ball a little so it doesn't get stuck, in case the ball did hit a block
        // prevents it from getting stuck
        if (!attachedStatus) {
            circle.move(velocity.x * .5f, velocity.y * .5f);
        }
    }
    if (velocity.x > BALLS_SPEED || velocity.y > BALLS_SPEED) {
        velocity.x *= .99;
        velocity.y *= .99;
    }
    circle.move(velocity);
}
const char Ball::collision(sf::FloatRect &boundingBox) const { return 'n'; }
bool Ball::isAttached() { return attachedStatus; }
void Ball::setVelocity(const sf::Vector2f &vel) { velocity = vel; }
void Ball::throwBall() {
    velocity.y = -cos(game.getDirection()) * BALLS_SPEED;
    velocity.x = -sin(game.getDirection()) * BALLS_SPEED;
    velocity.x += ((rand() % 100) - 50) / 1000.; // slightly deviate the ball (useful for special random ball)
    attachedStatus = false;
}
bool Ball::frameCollision(const sf::FloatRect &bounds) {
    sf::FloatRect boundsCopy = bounds;
    char c = game.getFrame()->collision(boundsCopy); // collision type
    return simpleCollision(c);
}
bool Ball::simpleCollision(const char &c) {
    if (c == 'v') {
        velocity.y *= -1;
        return true;
    } else if (c=='h') {
        velocity.x *= -1;
        return true;
    } else {
        return false;
    }
}
bool Ball::blockCollision(const sf::FloatRect& bounds, std::vector<Object*>& objects) {
    for (int i=game.indexOfFirstBlock; i<game.numBlocks; i++){
        sf::FloatRect boundsCopy = bounds;
        char c=objects[i]->collision(boundsCopy);
        if (c!='n'){
            objects[i]->decrementValue();
            if (objects[i]->getValue()<1){
                objects[i]->remove=true;
            }
        }
        if (simpleCollision(c)) {return true;}
        else if (c=='c') {
            float contactAngle=getContactAngle(circle.getPosition(), boundsCopy);
            sf::Transform rotation;
            rotation.rotate(contactAngle);
            velocity=rotation.transformPoint(velocity);
            velocity.y*=-1;
            rotation.rotate(-2*contactAngle);
            velocity=rotation.transformPoint(velocity);
            return true;
        }
    }
    return false;
}
float Ball::getContactAngle(const sf::Vector2f& object1, const sf::FloatRect& object2) const {
    float contactAngle;
    float aux=object1.y-object2.top;
    if (aux==0) {// π/2
        contactAngle = 1.570796327f;
    } else {
        contactAngle =
        atanf((object1.x-object2.left)/aux);
    }
    return contactAngle*RAD_TO_DEG;//in degrees 4 rotate finction
}
int Ball::getValue(){return 1;};
void Ball::decrementValue(){};
sf::CircleShape Ball::getCircle(){return circle;};
sf::Vector2f Ball::getVelocity(){ return velocity;};
