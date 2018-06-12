//
//  Frame.hpp
//  Projet JIN4
//
//  Created by Lucas on 05/06/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#ifndef Frame_hpp
#define Frame_hpp

#include "Object.hpp"

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
#endif /* Frame_hpp */
