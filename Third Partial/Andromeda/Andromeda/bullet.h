//
//  bullet.h
//  test
//
//  Created by Andres Garza on 08/05/17.
//  Copyright © 2017 Andres Garza. All rights reserved.
//

#ifndef bullet_h
#define bullet_h

#pragma once

class Bullet {
    
public:
    Bullet(float size) {
        bullet.setRadius(size);
        bullet.setFillColor(sf::Color::Red);
    }
    
    void fire(float speedx, float speedy) {
        bullet.move(speedx, speedy);
    }
    
    int getRight() {
        return bullet.getPosition().x + bullet.getRadius();
    }
    
    int getLeft() {
        return bullet.getPosition().x;
    }
    
    int getTop() {
        return bullet.getPosition().y;
    }
    
    int getBottom() {
        return bullet.getPosition().y + bullet.getRadius();
    }
    
    void draw(sf::RenderWindow &window) {
        window.draw(bullet);
    }
    
    void setPos(float x, float y) {
        bullet.setPosition(x, y);
    }
    
private:
    sf::CircleShape bullet;
};

#endif /* bullet_h */
