//
//  lool.cpp
//  Projet JIN4
//
//  Created by Lucas on 31/05/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#include "lool.hpp"
void load_settings(std::string file_name){
    pugi::xml_document doc;
    doc.load_file(file_name.c_str());
    pugi::xml_node node_settings=doc.child("Settings");
    if (!node_settings){
        std::cerr<<"Could not open file "<<file_name<<std::endl;
        std::cout<<"Default settings will be used instead\n";
    }else{
        BALLS_SPEED=node_settings.attribute("BALLS_SPEED").as_float();
        BALL_SIZE=node_settings.attribute("BALL_SIZE").as_float();
        LAUNCHER_SIZE=node_settings.attribute("LAUNCHER_SIZE").as_float();
        LAUNCHER_PRECISION=node_settings.attribute("LAUNCHER_PRECISION").as_float();
        DIRECTION_MAX=node_settings.attribute("DIRECTION_MAX").as_float();
        NB_BLOCKS_HORIZONTAL=node_settings.attribute("NB_BLOCKS_HORIZONTAL").as_float();
        NB_BLOCKS_VERTICAL=node_settings.attribute("NB_BLOCKS_VERTICAL").as_float();
        DEFAULT_BLOCK_VALUE=node_settings.attribute("DEFAULT_BLOCK_VALUE").as_float();
    }
}
int lool(){
    load_settings("settings.xml");
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "lool", sf::Style::Default, settings);
    window.setFramerateLimit(300);
    std::vector<Object *> objects;
    Game game = Game(window, objects);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            game.handleEvent(event);
        }
        game.update();
    }
    return EXIT_SUCCESS;
}
