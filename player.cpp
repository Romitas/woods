#include "stdafx.h"
#include "player.h"
#include "map.h"

Agent::Agent (sf::RenderWindow* window, Map* map) {
    this->window = window;
    this->map = map;

    this->X = rand() % 20;
    this->Y = rand() % 20;

    this->dest_x = this->X;
    this->dest_y = this->Y;

    this->step_x = this->X;
    this->step_y = this->Y;

    this->speed = 0.1;
    this->is_active = false;
    this->direction = NONE;
    this->view_direction = NONE;

    this->color = RED;

    this->rect.setTexture(&TEXTURE_ENEMY);

    this->glow_rect.setTexture(&TEXTURE_GLOW);
    this->glow_rect.setFillColor(LIGHT);

    this->glow_size = PLAYER_WIDTH * MIN_GLOW_SIZE;
}

bool Agent::isAtDestination() {
    return (this->X == this->dest_x && this->Y == this->dest_y);
}

void Agent::setNextStep(int direction) {
    std::vector<std::function<void()>> bypasses;
    bypasses.reserve(2);

    if (direction == UP) {
        this->step_y -= this->STEP;
    } else if (direction == DOWN) {
        this->step_y += this->STEP;
    } else if (direction == LEFT) {
        this->step_x -= this->STEP;
    } else if (direction == RIGHT) {
        this->step_x += this->STEP;

    } else if (direction == UPLEFT) {
        this->step_x -= this->STEP;
        this->step_y -= this->STEP;
    } else if (direction == UPRIGHT) {
        this->step_x += this->STEP;
        this->step_y -= this->STEP;
    } else if (direction == DOWNLEFT) {
        this->step_x -= this->STEP;
        this->step_y += this->STEP;
    } else if (direction == DOWNRIGHT) {
        this->step_x += this->STEP;
        this->step_y += this->STEP;
    }

    if (this->step_x < 0) {
        this->step_x = 0;
    } else if (this->step_x >= MAP_WIDTH) {
        this->step_x = MAP_WIDTH - 1;
    } else if (this->step_y < 0) {
        this->step_y = 0;
    } else if (this->step_y >= MAP_HEIGHT) {
        this->step_y = MAP_HEIGHT - 1;
    }

    if (!this->map->isAvailable(step_x, step_y)) {

        if (direction == UP || direction == DOWN) {
            if (this->map->isAvailable(step_x - 1, step_y)) {
                bypasses.push_back([&step_x = this->step_x, &dest_x = this->dest_x](){ step_x -= 1; dest_x -= 1;});
            } if (this->map->isAvailable(step_x + 1, step_y)) {
                bypasses.push_back([&step_x = this->step_x, &dest_x = this->dest_x](){ step_x += 1; dest_x += 1;});
            }
        } else if (direction == LEFT || direction == RIGHT) {
            if (this->map->isAvailable(step_x, step_y - 1)) {
                bypasses.push_back([&step_y = this->step_y, &dest_y = this->dest_y](){ step_y -= 1; dest_y -= 1;});
            } if (this->map->isAvailable(step_x, step_y + 1)) {
                bypasses.push_back([&step_y = this->step_y, &dest_y = this->dest_y](){ step_y += 1; dest_y += 1;});
            }
        } else if (direction == UPLEFT) {
            if (this->map->isAvailable(step_x, step_y + 1)) {
                bypasses.push_back([&step_y = this->step_y](){ step_y += 1; });
            } if (this->map->isAvailable(step_x + 1, step_y)) {
                bypasses.push_back([&step_x = this->step_x](){ step_x += 1; });
            }
        } else if (direction == UPRIGHT) {
            if (this->map->isAvailable(step_x - 1, step_y)) {
                bypasses.push_back([&step_x = this->step_x](){ step_x -= 1; });
            } if (this->map->isAvailable(step_x, step_y + 1)) {
                bypasses.push_back([&step_y = this->step_y](){ step_y += 1; });
            }
        } else if (direction == DOWNLEFT) {
            if (this->map->isAvailable(step_x, step_y - 1)) {
                bypasses.push_back([&step_y = this->step_y](){ step_y -= 1; });
            } if (this->map->isAvailable(step_x + 1, step_y)) {
                bypasses.push_back([&step_x = this->step_x](){ step_x += 1; });
            }
        } else if (direction == DOWNRIGHT) {
            if (this->map->isAvailable(step_x - 1, step_y)) {
                bypasses.push_back([&step_x = this->step_x](){ step_x -= 1; });
            } if (this->map->isAvailable(step_x, step_y - 1)) {
                bypasses.push_back([&step_y = this->step_y](){ step_y -= 1; });
            }
        }

        if (bypasses.empty()) {
            this->step_x = this->X;
            this->step_y = this->Y;
        } else {
            choice<std::function<void()>>(bypasses)();
        }
    }
}

void Agent::animateToDestination() {
    int direction;

    if (dist(this->X, this->step_x) < this->speed && dist(this->Y, this->step_y) < this->speed) {
        this->X = this->step_x;
        this->Y = this->step_y;

        direction = NONE;

        if (this->X == this->dest_x && this->Y > this->dest_y) {
            direction = UP;
        } else if (this->X == this->dest_x && this->Y < this->dest_y) {
            direction = DOWN;
        } else if (this->X > this->dest_x && this->Y == this->dest_y) {
            direction = LEFT;
        } else if (this->X < this->dest_x && this->Y == this->dest_y) {
            direction = RIGHT;

        } else if (this->X > this->dest_x && this->Y > this->dest_y) {
            direction = UPLEFT;
        } else if (this->X < this->dest_x && this->Y > this->dest_y) {
            direction = UPRIGHT;
        } else if (this->X > this->dest_x && this->Y < this->dest_y) {
            direction = DOWNLEFT;
        } else if (this->X < this->dest_x && this->Y < this->dest_y) {
            direction = DOWNRIGHT;
        }
        
        if (direction != NONE) {
            this->view_direction = direction;
        }

        this->map->get(this->step_x, this->step_y)->trace_direction = direction;
        this->setNextStep(direction);
    }

    if (dist(this->X, this->dest_x) < this->speed && dist(this->Y, this->dest_y) < this->speed) {
        this->X = this->dest_x;
        this->Y = this->dest_y;
    } else {
        if (this->X < this->step_x) {
            this->X += this->speed;
        } else if (this->X > this->step_x) {
            this->X -= this->speed;
        } if (this->Y < this->step_y) {
            this->Y += this->speed;
        } else if (this->Y > this->step_y) {
            this->Y -= this->speed;
        }
    }
}

void Agent::setDirection(int direction) {
    if (dist(this->X, this->step_x) < this->speed && dist(this->Y, this->step_y) < this->speed) {
        this->direction = direction;
        this->view_direction = direction;

        if (direction == UP) {
            this->setDestination(this->X, -1);
        } else if (direction == DOWN) {
            this->setDestination(this->X, MAP_HEIGHT);
        } else if (direction == LEFT) {
            this->setDestination(-1, this->Y);
        } else if (direction == RIGHT) {
            this->setDestination(MAP_WIDTH, this->Y);
        } else if (direction == UPLEFT) {
            this->setDestination(-1, -1);
        } else if (direction == UPRIGHT) {
            this->setDestination(MAP_WIDTH, -1);
        } else if (direction == DOWNLEFT) {
            this->setDestination(-1, MAP_HEIGHT);
        } else if (direction == DOWNRIGHT) {
            this->setDestination(MAP_WIDTH, MAP_HEIGHT);
        } else {
            this->setNextStep(direction);
            this->setDestination(this->step_x, this->step_y);
        }
    }
}

void Agent::setDestination(int dest_x, int dest_y) {
    if (dest_x < 0) {
        dest_x = 0;
    } else if (dest_x >= MAP_WIDTH) {
        dest_x = MAP_WIDTH - 1;
    } if (dest_y < 0) {
        dest_y = 0;
    } else if (dest_y >= MAP_HEIGHT) {
        dest_y = MAP_HEIGHT - 1;
    } 

    this->dest_x = dest_x;
    this->dest_y = dest_y;
}

void Agent::update(float time) {
    int x, y;

    if (this->isAtDestination()) {
        do {
            x = this->X + ((rand() % 40) - 20);
            y = this->Y + ((rand() % 40) - 20);
        } while (!this->map->isAvailable(x, y));

        this->setDestination(x, y);
    } else {
        this->animateToDestination();
    } 
}

int Agent::getAgentRotation() {
    if (this->view_direction == NONE) {
        return 0;
    } else {
        return this->view_direction;
    }
}


sf::Vector2i Agent::getScreenCoordinates(float px, float py) {
    sf::Vector2f scale = this->map->get_scale();
    float gridWidth = scale.x;
    float gridHeight = scale.y;

    float dist_x = (gridWidth - 1) * 0.5;
    float dist_y = (gridHeight - 1) * 0.5;

    float left = px - dist_x;
    float top = py - dist_y;
    float right = px + dist_x;
    float bottom = py + dist_y;

    if (left < 0) {
        left = 0;
        right = gridWidth - 1;
    } if (top < 0) {
        top = 0;
        bottom = gridHeight - 1;
    } if (right > MAP_WIDTH - 1) {
        right = MAP_WIDTH - 1;
        left = MAP_WIDTH - gridWidth;
    } if (bottom > MAP_HEIGHT - 1) {
        bottom = MAP_HEIGHT - 1;
        top = MAP_HEIGHT - gridHeight;
    }

    if (this->X < left || this->X > right || this->Y < top || this->Y > bottom) {
        return sf::Vector2i(NONE, NONE);
    }

    int screen_x = (this->X - left) * CELL_WIDTH + CELL_WIDTH / 2;
    int screen_y = SKY_HEIGHT + (this->Y - top) * CELL_HEIGHT + CELL_HEIGHT / 2;

    return sf::Vector2i(screen_x, screen_y);
}

void Agent::draw(float px, float py) {
    if (this->map->get(this->X, this->Y)->fog) {
        return;
    }

    sf::Vector2i screen_coordinates = this->getScreenCoordinates(px, py);
    if (screen_coordinates.x == NONE || screen_coordinates.y == NONE) {
        return;
    }

    this->rect.setSize(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT));
    this->rect.setOrigin(PLAYER_WIDTH / 2, PLAYER_HEIGHT - CELL_HEIGHT / 4);
    this->rect.setPosition(screen_coordinates.x, screen_coordinates.y);
    this->rect.setRotation(this->getAgentRotation());

    this->window->draw(this->rect);
}

Player::Player (sf::RenderWindow* window, Map* map) : Agent (window, map) {
    map->player = this; // oh god =/

    this->speed = 0.2;

    this->X = 0;
    this->Y = 0;

    this->dest_x = this->X;
    this->dest_y = this->Y;

    this->step_x = this->X;
    this->step_y = this->Y;

    this->rect.setTexture(&TEXTURE_PLAYER);
}

void Player::updateFog() {
    int left = this->X - VIEW_RADIUS;
    int top = this->Y - VIEW_RADIUS;
    int right = this->X + VIEW_RADIUS;
    int bottom = this->Y + VIEW_RADIUS;

    if (left < 0) {
        left = 0;
    } if (top < 0) {
        top = 0;
    } if (right > MAP_WIDTH - 1) {
        right = MAP_WIDTH - 1;
    } if (bottom > MAP_HEIGHT - 1) {
        bottom = MAP_HEIGHT - 1;
    }

    for (int i = left; i <= right; i++) {
        for (int j = top; j <= bottom; j++) {
            if (dist(i, this->X) + dist(j, this->Y) < VIEW_RADIUS) {
                this->map->get(i, j)->fog = false;
            }
        }
    }
}

void Player::updateGlowSize() {
    this->glow_size += rand() % 20 - 10;
    if (this->glow_size < PLAYER_WIDTH * MIN_GLOW_SIZE) {
        this->glow_size = PLAYER_WIDTH * MIN_GLOW_SIZE;
    } else if (this->glow_size > PLAYER_WIDTH * MAX_GLOW_SIZE) {
        this->glow_size = PLAYER_WIDTH * MAX_GLOW_SIZE;
    }

    this->glow_rect.setSize(sf::Vector2f(this->glow_size, this->glow_size));
    this->glow_rect.setOrigin(this->glow_size / 2, this->glow_size / 2);
}

void Player::update(float time) {
    bool up = false, down = false, left = false, right = false;
    int direction; 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        up = true;
    } if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        down = true;
    } if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        left = true;
    } if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        right = true;
    } 

    if (up || down || right || left) {
        this->is_active = true;

        if (up && left) {
            direction = UPLEFT;
        } else if (up && right) {
            direction = UPRIGHT;
        } else if (down && left) {
            direction = DOWNLEFT;
        } else if (down && right) {
            direction = DOWNRIGHT;
        } else if (up) {
            direction = UP;
        } else if (down) {
            direction = DOWN;
        } else if (left) {
            direction = LEFT;
        } else if (right) {
            direction = RIGHT;
        } else {
            direction = NONE;
        }
    } else {
        this->is_active = false;
        this->direction = NONE;
        this->setNextStep(this->direction);
        this->setDestination(this->step_x, this->step_y);
    }

    if (this->is_active && this->direction != direction) {
        this->setDirection(direction);
    }

    if (!this->isAtDestination()) {
        this->animateToDestination();
    }

    this->updateFog();
}

void Player::draw_glow() {
    this->updateGlowSize();
    sf::Vector2i screen_coordinates = this->getScreenCoordinates(this->X, this->Y);
    this->glow_rect.setPosition(screen_coordinates.x, screen_coordinates.y);
    this->window->draw(this->glow_rect);
}

void Player::draw_player() {
    this->draw(this->X, this->Y);
}
