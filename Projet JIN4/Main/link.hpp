//
//  inclusion.hpp
//  Projet JIN4
//
//  Created by Lucas on 04/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#ifndef link_hpp
#define link_hpp

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <map>
#include <cassert>
#include <list>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "pugixml.hpp"

/*settings*/
extern float BALLS_SPEED;
extern float BALL_SIZE;
extern float LAUNCHER_SIZE;
extern float LAUNCHER_PRECISION;
extern float DIRECTION_MAX;
extern unsigned int NB_BLOCKS_HORIZONTAL;
extern unsigned int NB_BLOCKS_VERTICAL;
extern float DEFAULT_BLOCK_VALUE;

/*constants*/
static const float RAD_TO_DEG=57.295779513;
const unsigned int WINDOW_WIDTH=1100;
const unsigned int WINDOW_HEIGHT=1600;
const float TOOLBAR_HEIGHT=30;
const float FRAME_WIDTH=40;
const float FRAME_BUFFER=0;
const float BLOCK_HEADROOM=2;
const float REMOVAL_MARGIN=BALL_SIZE+BALLS_SPEED;/*How far out of the frame a ball must be before being removed. Necessary to prevent the ball from being removed before being launched. */

const sf::Color DEFAULT_COLOR = sf::Color(51,51,51);
const sf::Color BACKGROUND_COLOR = sf::Color(245,245,245);
const sf::Color BALL_COLOR = sf::Color(122,122,122);
const sf::Color BLOCK_COLOR = sf::Color(25,130,196);
const sf::Color SPECIAL_BLOCK_COLOR = sf::Color(255,202,58);
const sf::Color LOSE_COLOR = sf::Color(255,0,0);
const sf::Color WIN_COLOR = sf::Color(138,201,38);

void readxml();

#endif /* link_hpp */
