#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"

class Agent {
protected:
    Map *map;

    sf::RenderWindow* window;
    sf::RectangleShape rect;
    sf::RectangleShape glow_rect;
    sf::Color color;

    const int STEP = 1;
    float speed;
    
    int direction;
    int view_direction;

    int glow_size;

public:
    int dest_x;
    int dest_y;

    int step_x;
    int step_y;

    bool is_active;

    float X;
    float Y;

    Agent (sf::RenderWindow* window, Map* map);
    bool isAtDestination();
    void setNextStep(int direction);
    void animateToDestination();
    void setDirection(int direction);
    void setDestination(int dest_x, int dest_y);

    virtual void update(float time);
    int getAgentRotation();
    sf::Vector2i getScreenCoordinates(float px, float py);
    void draw(float px, float py);
};

class Player : public Agent {
public:
    Player (sf::RenderWindow* window, Map* map);
    void updateFog();
    void updateGlowSize();
    void update(float time);
    void draw_glow();
    void draw_player();
};

#endif
