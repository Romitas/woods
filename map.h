#ifndef MAP_H
#define MAP_H

#include "stdafx.h"
#include "props.hpp"

class Player;

class Cell {
    sf::RenderWindow* window;
    int terrain;

public:

    bool has_prop;
    int prop_type;
    bool fog;
    int trace_direction;

    Prop* prop;
    sf::Color color;

    Cell(sf::RenderWindow* window);
    ~Cell();

    void setProp(Prop* prop);
    bool isAvailable();

    void setTerrain(int terrain);
};

class Map {
    int viewmode;

    sf::RenderWindow* window;

    std::vector<Cell*> map;
    std::vector<Cell*> fog;
    std::vector<Prop*> props;

    sf::Color light_color;

    Gold *gold;

    float gridWidth;
    float gridHeight;

    bool isSwitchingViewmode;
    bool isScaling;

    int daytime;
    int hour;
    int minutes;

    void animateToColor(sf::Color dest, int time);
    void putGold();
    void generate();

public:
    Player* player;
    Map(sf::RenderWindow* window);
    sf::Vector2f get_scale();
    Cell* get(int x, int y);

    int curDaytime();
    bool isAvailable(int x, int y);
    void tick();
    void switchViewmode();
    void update(float time);
    void draw(float px, float py, int dx, int dy);
};

#endif
