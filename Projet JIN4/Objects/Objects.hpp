//
//  Objects.hpp
//  Projet JIN4
//
//  Created by Lucas on 06/07/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#ifndef Objects_hpp
#define Objects_hpp

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

class Game {
public:
    Game(sf::RenderWindow &window, std::vector<Object *> &objects);
    void update();
    void handleEvent(sf::Event &event);
    void throwBall();
    std::vector<Object *> &getObjects();
    Object *getFrame() const;
    float getDirection();
    int indexOfFirstBlock;
    long numBlocks;
    sf::Vector2u windowSize;
    
private:
    std::vector<Object *> &objects;
    sf::RenderWindow &window;
    sf::Event latestEvent;
    float direction;
    sf::CircleShape launcher;
    char status; //'r'=ready, 'p'=playing
    
    void clear();
    void newLevel();
    //void gameOver(bool won = false);
    void start();
};

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

class Frame : public Object {
public:
    Frame(float windowWidth, float windowHeight, float width, float buffer, sf::Color color = DEFAULT_COLOR);
    void draw(sf::RenderWindow& window) const;
    const char collision(sf::FloatRect& boundingBox) const;
    int getValue();
    void decrementValue();
private:
    sf::RectangleShape left;
    sf::RectangleShape top;
    sf::RectangleShape right;
};

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

#endif /* Objects_hpp */
