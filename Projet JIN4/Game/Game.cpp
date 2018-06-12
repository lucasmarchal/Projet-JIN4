//
//  Game.cpp
//  Projet JIN4
//
//  Created by Lucas on 05/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#include "Game.hpp"
#include "Frame.hpp"
#include "Ball.hpp"
#include "Block.hpp"
Game::Game(sf::RenderWindow &window, std::vector<Object *> &objects)
: window(window), windowSize(window.getSize()), objects(objects) {
    direction = 0;
    status = 'r';
    launcher = sf::CircleShape(LAUNCHER_SIZE, 3);
    launcher.setOrigin(LAUNCHER_SIZE, LAUNCHER_SIZE);
    newLevel();
}
void Game::update() {
    window.clear(BACKGROUND_COLOR);
    if (status == 'r') {
        launcher.setFillColor(WIN_COLOR);
        window.draw(launcher);
    } else {
        launcher.setFillColor(LOSE_COLOR);
        window.draw(launcher);
    }
    int index = 0;
    for (auto i = objects.begin(); i != objects.end(); i++) { // objects
        Object *object = *i;
        if (object->remove) {
            delete object;
            i = objects.erase(i);
            --i; // i will be incremented by the for loop
            if (index >= indexOfFirstBlock && index < numBlocks) {
                numBlocks--;
            }
            continue;
        }
        object->draw(window);
        object->move();
        index++;
    }
    if (objects.size() == numBlocks + indexOfFirstBlock) {
        objects.push_back(new Ball(*this, BALL_COLOR, BALL_SIZE));
        objects.push_back(new Ball(*this, BALL_COLOR, BALL_SIZE));
        status = 'r';
    }
    window.display();
}
void Game::handleEvent(sf::Event &event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    if (status == 'r' && event.type == sf::Event::KeyPressed) {
        sf::Keyboard::Key &key = event.key.code;
        if (key == sf::Keyboard::W || key == sf::Keyboard::X) {
            switch (key) {
                case sf::Keyboard::W:
                    if (direction < DIRECTION_MAX) {
                        direction += LAUNCHER_PRECISION;
                    }
                    break;
                case sf::Keyboard::X:
                    if (direction > -DIRECTION_MAX) {
                        direction -= LAUNCHER_PRECISION;
                    }
                    break;
                default:
                    break;
            }
        } else {
            switch (key) {
                case sf::Keyboard::S:
                    status = 'p';
                    throwBall();
                    break;
                case sf::Keyboard::Escape:
                    // TODO : pause the game
                    window.close();
                    break;
                default:
                    break;
            }
        }
    } else if (event.type == sf::Event::MouseWheelMoved) {
        direction = direction + LAUNCHER_PRECISION * event.mouseWheel.delta;
        direction = fmin(direction, DIRECTION_MAX);
        direction = fmax(direction, -DIRECTION_MAX);
    }
    launcher.setPosition(WINDOW_WIDTH / 2 - direction * (WINDOW_WIDTH - FRAME_WIDTH) / (2 * DIRECTION_MAX), WINDOW_HEIGHT);
}
void Game::throwBall() {
    for (long i = numBlocks + indexOfFirstBlock; i < objects.size(); i++) {
        Ball *ball = dynamic_cast<Ball *>(objects[i]);
        if (ball->isAttached()) {
            ball->throwBall();
            // break;
        }
    }
}
void Game::clear() {
    auto i = objects.end();
    i--;
    while (objects.size() > indexOfFirstBlock) {
        delete *i;
        i--;
        objects.pop_back();
    }
}
void Game::newLevel() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    objects.push_back(new Frame(windowWidth, windowHeight, FRAME_WIDTH, FRAME_BUFFER));
    indexOfFirstBlock = int(objects.size());
    
    float blockWidth = (((WINDOW_WIDTH - 2 * FRAME_WIDTH) - BLOCK_HEADROOM) / NB_BLOCKS_HORIZONTAL);
    float blockHeight = blockWidth;
    sf::Vector2f origin = sf::Vector2f(FRAME_BUFFER + FRAME_WIDTH, TOOLBAR_HEIGHT + FRAME_BUFFER + FRAME_WIDTH);
    for (int row = 0; row < NB_BLOCKS_VERTICAL; row++) {
        for (int col = 0; col < NB_BLOCKS_HORIZONTAL; col++) {
            objects.push_back(new Block(origin.x + BLOCK_HEADROOM + blockWidth * col, origin.y + BLOCK_HEADROOM + row * (blockHeight + BLOCK_HEADROOM), blockWidth - BLOCK_HEADROOM, blockHeight, DEFAULT_BLOCK_VALUE));
        }
    }
    numBlocks = objects.size() - indexOfFirstBlock;
    objects.push_back(new Ball(*this, BALL_COLOR, BALL_SIZE));
    objects.push_back(new Ball(*this, BALL_COLOR, BALL_SIZE));
}
std::vector<Object *> &Game::getObjects() { return objects; }
Object *Game::getFrame() const {
    return objects[0];
} // the frame is the first object
float Game::getDirection() { return direction; };
