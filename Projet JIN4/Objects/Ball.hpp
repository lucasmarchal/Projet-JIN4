//
//  Ball.hpp
//  Projet JIN4
//
//  Created by Lucas on 05/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#ifndef Ball_hpp
#define Ball_hpp

#include "Object.hpp"
#include "Game.hpp"
class Ball : public Object {
public:
    Ball(Game& game, float xPos, float yPos, float xVel, float yVel, sf::Color color = BALL_COLOR,float radius = BALL_SIZE);
    Ball(Game& game, sf::Color color=BALL_COLOR, float radius=BALL_SIZE);

    void draw(sf::RenderWindow& window) const;
    void move();
    const char collision(sf::FloatRect& boundingBox) const;
    void setVelocity(const sf::Vector2f& velocity);
    void throwBall();
    bool isAttached();
    int getValue();
    void decrementValue();
    sf::CircleShape getCircle();
    sf::Vector2f getVelocity();

private:
    bool frameCollision(const sf::FloatRect &bounds);
    bool simpleCollision(const char& c);//when you just needa invert the x or y velocity
    bool blockCollision(const sf::FloatRect& bounds, std::vector<Object*>& objects);
    float getContactAngle(const sf::Vector2f& object1, const sf::FloatRect& object2) const;
    bool attachedStatus;//b4 launching
    sf::CircleShape circle;//shape drawn in the window
    sf::Vector2f velocity;
    Game& game;//the game in which the ball is being launched
};
#endif /* Ball_hpp */
