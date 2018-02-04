#ifndef PROPS_H
#define PROPS_H

#include "stdafx.h"

class Prop {
protected:
    sf::RenderWindow* window;
    sf::RectangleShape rect;

public:
    int type;

    int x;
    int y;

    float width;
    float height;

    sf::Color color;
    sf::Texture* texture;

    Prop() {}
    Prop(sf::RenderWindow* window, int x, int y) {
        this->window = window;

        this->x = x;
        this->y = y;

        this->width = 0.2;
        this->height = 0.2;

        this->type = PROP_OBSTACLE;
        this->color = BLACK;
        this->rect.setFillColor(this->color);
    }

    virtual void draw(int cell_x, int cell_y) {
        this->rect.setPosition(cell_x + (CELL_WIDTH - CELL_WIDTH * this->width) / 2, cell_y + (CELL_HEIGHT - CELL_HEIGHT * this->height) / 2);
        this->rect.setSize(sf::Vector2f(CELL_WIDTH * this->width, CELL_HEIGHT * this->height));

        this->window->draw(this->rect);
    }
};

class Gold : public Prop {

public:
    Gold() : Prop() {}
    Gold(sf::RenderWindow* window, int x, int y) : Prop(window, x, y) {
        this->type = PROP_COLLECTIBLE;
        this->texture = &TEXTURE_GOLD;
        this->color = GOLD;

        this->width = 0.6;
        this->height = 0.6;

        this->rect.setTexture(this->texture);
        this->rect.setFillColor(this->color);
    }

    void draw(int cell_x, int cell_y) {
        this->rect.setPosition(cell_x + (CELL_WIDTH - CELL_WIDTH * this->width) / 2, cell_y + (CELL_HEIGHT - CELL_HEIGHT * this->height) / 2);
        this->rect.setSize(sf::Vector2f(CELL_WIDTH * this->width, CELL_HEIGHT * this->height));

        this->window->draw(this->rect);
    }
};

/*
class PropTree : public Prop {
    public:

    PropTree() : Prop() {}
    PropTree(sf::RenderWindow* window, int x, int y) : Prop(sf::RenderWindow* window, int x, int y) {
        this->color = DIRT5;
    }
};

class PropStone : public Prop {
    public:

    PropStone() : Prop() {}
    PropStone(sf::RenderWindow* window, int x, int y) : Prop(sf::RenderWindow* window, int x, int y) {
        this->width = 2;
        this->height = 2;

        this->color = DIRT5;
    }
};
*/

#endif
