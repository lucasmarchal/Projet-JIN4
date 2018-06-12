//
//  googleTest.cpp
//  UnitTests
//
//  Created by Lucas on 15/05/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#include <iostream>
#include "gtest/gtest.h"
#include "../Projet JIN4/Main/lool.hpp"
#include "Ball.hpp"

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    int aux=RUN_ALL_TESTS();
    return aux;
}
TEST(XML,loadSettings){
    load_settings("test_settings.xml");
    EXPECT_FLOAT_EQ(BALLS_SPEED, 4.);
    EXPECT_FLOAT_EQ(BALL_SIZE,12);
    EXPECT_FLOAT_EQ(LAUNCHER_SIZE,44);
    EXPECT_FLOAT_EQ(LAUNCHER_PRECISION,0.02);
    EXPECT_FLOAT_EQ(DIRECTION_MAX,1.1);
    EXPECT_FLOAT_EQ(NB_BLOCKS_HORIZONTAL,6);
    EXPECT_FLOAT_EQ(NB_BLOCKS_VERTICAL,6);
    EXPECT_FLOAT_EQ(DEFAULT_BLOCK_VALUE,5);
}
TEST(Game,init){
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "lool_test", sf::Style::Default, settings);
    std::vector<Object *> objects;
    Game game = Game(window, objects);
    
    EXPECT_EQ(objects, game.getObjects());
    EXPECT_NE(nullptr, game.getFrame());
    EXPECT_EQ(game.getDirection(), 0);
}
TEST(Game,level){
    load_settings("test_settings.xml");
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "lool_test", sf::Style::Default, settings);
    std::vector<Object *> objects;
    Game game = Game(window, objects);
    EXPECT_EQ(game.indexOfFirstBlock, 1);
    EXPECT_EQ(game.numBlocks-1, 36);
    EXPECT_EQ(game.getObjects().size(), 38);
}
TEST(Ball,init){
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "lool_test", sf::Style::Default, settings);
    std::vector<Object *> objects;
    Game game = Game(window, objects);
    Ball *ball=new Ball(game, 50, 52, 10, 12);
    EXPECT_FLOAT_EQ(ball->getVelocity().x, 10);
    EXPECT_FLOAT_EQ(ball->getVelocity().y, 12);
    EXPECT_FLOAT_EQ(ball->getCircle().getPosition().x, 50);
    EXPECT_FLOAT_EQ(ball->getCircle().getPosition().y, 52);
    EXPECT_EQ(ball->isAttached(), false);
}
TEST(Ball,throw_){
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "lool_test", sf::Style::Default, settings);
    std::vector<Object *> objects;
    Game game = Game(window, objects);
    Ball *ball=new Ball(game);
    EXPECT_EQ(ball->isAttached(), true);
    EXPECT_FLOAT_EQ(ball->getVelocity().x, 0);
    EXPECT_FLOAT_EQ(ball->getVelocity().y, 0);
    EXPECT_FLOAT_EQ(ball->getCircle().getPosition().x, WINDOW_WIDTH/2);
    EXPECT_FLOAT_EQ(ball->getCircle().getPosition().y, WINDOW_HEIGHT);
}


