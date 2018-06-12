//
//  Game.hpp
//  Projet JIN4
//
//  Created by Lucas on 05/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include "link.hpp"
#include "Object.hpp"
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
#endif /* Game_hpp */
