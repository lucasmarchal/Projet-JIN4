//
//  Objects.cpp
//  Projet JIN4
//
//  Created by Lucas on 06/07/2018.
//  Copyright © 2018 Lucas Marchal. All rights reserved.
//

#include "Objects.hpp"

/****BALL****/

Ball::Ball(Game &game, float x_position, float y_position, float x_velocity, float y_velocity, sf::Color color, float radius)
: game(game), circle(radius, 50), velocity(x_velocity, y_velocity), attachedStatus(false) {
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
    circle.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
}
void Ball::draw(sf::RenderWindow &window) const { window.draw(circle); }
void Ball::move() {
    sf::FloatRect bounds = circle.getGlobalBounds();
    if (bounds.top > game.windowSize.y + REMOVAL_MARGIN) { // mark for removal if the ball has left the screen
        remove = true;
        return;
    }
    std::vector<Object *> &objects = game.getObjects();
    if (attachedStatus || (frameCollision(bounds)) || (blockCollision(bounds, objects))) {
        // moves the ball a little so it doesn't get stuck, in case the ball did hit a block (prevents it from getting stuck)
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
    velocity.x +=
    ((rand() % 100) - 50) /
    1000.; // slightly deviate the ball (to distinguish the two balls)
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
    } else if (c == 'h') {
        velocity.x *= -1;
        return true;
    } else {
        return false;
    }
}
bool Ball::blockCollision(const sf::FloatRect &bounds, std::vector<Object *> &objects) {
    for (int i = game.indexOfFirstBlock; i < game.numBlocks; i++) {
        sf::FloatRect boundsCopy = bounds;
        char c = objects[i]->collision(boundsCopy);
        if (c != 'n') {
            objects[i]->decrementValue();
            if (objects[i]->getValue() < 1) {
                objects[i]->remove = true;
            }
        }
        if (simpleCollision(c)) {
            return true;
        } else if (c == 'c') {
            float contactAngle = getContactAngle(circle.getPosition(), boundsCopy);
            sf::Transform rotation;
            rotation.rotate(contactAngle);
            velocity = rotation.transformPoint(velocity);
            velocity.y *= -1;
            rotation.rotate(-2 * contactAngle);
            velocity = rotation.transformPoint(velocity);
            return true;
        }
    }
    return false;
}
float Ball::getContactAngle(const sf::Vector2f &object1, const sf::FloatRect &object2) const {
    float contactAngle;
    float aux = object1.y - object2.top;
    if (aux == 0) { // π/2
        contactAngle = 1.570796327f;
    } else {
        contactAngle = atanf((object1.x - object2.left) / aux);
    }
    return contactAngle * RAD_TO_DEG; // in degrees 4 rotate function
}
int Ball::getValue() { return 1; };
void Ball::decrementValue(){};
sf::CircleShape Ball::getCircle() { return circle; };
sf::Vector2f Ball::getVelocity() { return velocity; };

/****FRAME****/

Frame::Frame(float windowWidth, float windowHeight, float width, float buffer, sf::Color color)
: left(sf::Vector2f(width, windowHeight - buffer - TOOLBAR_HEIGHT)),
top(sf::Vector2f(windowWidth - 2 * buffer - width, width)), // 2 buffers 4 extra space on left & left
right(sf::Vector2f(width, windowHeight - buffer - TOOLBAR_HEIGHT)) {
    left.setPosition(buffer, buffer + TOOLBAR_HEIGHT);
    top.setPosition(buffer, buffer + TOOLBAR_HEIGHT);
    right.setPosition(windowWidth - buffer - width, buffer + TOOLBAR_HEIGHT);
    left.setFillColor(color);
    top.setFillColor(color);
    right.setFillColor(color);
}
void Frame::draw(sf::RenderWindow &window) const {
    window.draw(left);
    window.draw(top);
    window.draw(right);
}
const char Frame::collision(sf::FloatRect &boundingBox) const {
    if (left.getGlobalBounds().intersects(boundingBox) ||
        right.getGlobalBounds().intersects(boundingBox)) {
        return 'h'; // horizontal
    } else if (top.getGlobalBounds().intersects(boundingBox)) {
        return 'v'; // vertical
    } else {
        return 'n'; // none
    }
}
int Frame::getValue() { return 1; };
void Frame::decrementValue(){};

/****BLOCK****/

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

/****GAME****/

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
