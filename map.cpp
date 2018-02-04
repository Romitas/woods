#include "stdafx.h"
#include "props.hpp"
#include "map.h"
#include "player.h"

Cell::Cell(sf::RenderWindow* window) {
    this->window = window;
    this->terrain = NONE;
    this->has_prop = false;
    this->prop_type = PROP_NONE;
    this->trace_direction = NONE;
    this->fog = true;
}

Cell::~Cell() { }

void Cell::setProp(Prop* prop) {
    this->prop = prop;
    this->has_prop = true;

    this->prop_type = prop->type;
}

bool Cell::isAvailable() {
    return !(this->prop_type == PROP_OBSTACLE);
}

void Cell::setTerrain(int terrain){
    this->terrain = terrain;
    switch (this->terrain) {
        case TERRAIN_GRASS:
            this->color = choice<sf::Color>(GRASS_COLORS);
            break;
        case TERRAIN_SNOW:
            this->color = choice<sf::Color>(SNOW_COLORS);
            break;
        default:
            this->color = SNOW1;
            break;
    }
}

void Map::animateToColor(sf::Color dest, int time) {
    int remains = ((time - this->hour - 1) * HOUR_LENGTH + (HOUR_LENGTH - this->minutes));
    this->light_color.r += (dest.r - this->light_color.r) / remains;
    this->light_color.g += (dest.g - this->light_color.g) / remains;
    this->light_color.b += (dest.b - this->light_color.b) / remains;
    this->light_color.a += (dest.a - this->light_color.a) / remains;
}

void Map::putGold() {
    int x, y;
    do {
        x = rand() % (MAP_WIDTH / 4);
        y = rand() % (MAP_HEIGHT / 4);
    } while (!this->isAvailable(x, y));

    this->gold = new Gold(window, x, y);
    this->get(x, y)->setProp(gold);
}

void Map::generate() {
    int border = MAP_HEIGHT / 2;
    int max_dist = MAP_HEIGHT / 6;
    int dist;
    int defaultTerrain = TERRAIN_SNOW;

    Cell *cell;

    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            cell = new Cell(window);
            dist = border - j;


            if (prob(0.5 + 0.5 * dist / max_dist)) {
                cell->setTerrain(TERRAIN_SNOW);
            } else {
                cell->setTerrain(TERRAIN_GRASS);
            }

            if (rand() % 10 < 1) {
                Prop *prop = new Prop(window, i, j);
                cell->setProp(prop);
                this->props.push_back(prop);
            } 

            this->map.push_back(cell);
        }
    }

    for (int i = 0; i < 10; i++) {
        this->putGold();
    }
}

Map::Map(sf::RenderWindow* window) {
    this->window = window;
    this->map.reserve(MAP_WIDTH * MAP_HEIGHT); 

    this->viewmode = VIEWMODE_TOP;
    this->isSwitchingViewmode = false;
    this->isScaling = false;

    this->gridWidth = GRID_WIDTH;
    this->gridHeight = GRID_HEIGHT;

    this->minutes = 0;
    this->hour = 12;
    this->daytime = DAYTIME_DAY;
    this->light_color = LIGHT_DAY;

    this->generate();
}

sf::Vector2f Map::get_scale() {
    return sf::Vector2f(this->gridWidth, this->gridHeight);
}

Cell* Map::get(int x, int y) {
    if (x > MAP_WIDTH - 1) {
        throw std::invalid_argument("X is more than MAP_WIDTH - 1");
    } else if (x < 0) {
        throw std::invalid_argument("X is less then 0");
    } if (y > MAP_HEIGHT - 1) {
        throw std::invalid_argument("Y is more than MAP_HEIGHT - 1");
    } else if (y < 0) {
        throw std::invalid_argument("Y is less then 0");
    }

    return this->map[MAP_HEIGHT * x + y];
}

int Map::curDaytime() {
    int res;
    if (this->hour >=0 && this->hour < 6) {
        res = DAYTIME_NIGHT;
    } else if (this->hour >=6 && this->hour < 9) {
        res = DAYTIME_MORNING;
    } else if (this->hour >=9 && this->hour < 18) {
        res = DAYTIME_DAY;
    } else if (this->hour >=18 && this->hour < 20) {
        res = DAYTIME_EVENING;
    } else {
        res = DAYTIME_NIGHT;
    }

    return res;
}



bool Map::isAvailable(int x, int y) {
    Cell* cell;

    try {
        cell = this->get(x, y);
    } catch (const std::invalid_argument &e) {
        return false;
    }

    return cell->isAvailable();
}

void Map::tick() {
    int was_daytime = this->daytime;

    this->minutes += 1;

    if (this->minutes >= HOUR_LENGTH) {
        this->minutes = 0;
        this->hour += 1;
    }

    if (this->hour >= 24) {
        this->hour = 0;
    }

    int is_daytime = this->curDaytime();

    if (was_daytime != is_daytime || (this->light_color != LIGHT_MORNING && this->light_color != LIGHT_DAY && this->light_color != LIGHT_EVENING && this->light_color != LIGHT_NIGHT)) {
        if (is_daytime == DAYTIME_MORNING) {
            //this->light_color = LIGHT_MORNING;
            animateToColor(LIGHT_MORNING, 8);
        } else if (is_daytime == DAYTIME_DAY) {
            //this->light_color = LIGHT_DAY;
            animateToColor(LIGHT_DAY, 12);
        } else if (is_daytime == DAYTIME_EVENING) {
            //this->light_color = LIGHT_EVENING;
            animateToColor(LIGHT_EVENING, 19);
        } else if (is_daytime == DAYTIME_NIGHT) {
            //this->light_color = LIGHT_NIGHT;
            animateToColor(LIGHT_NIGHT, 22);
        }
    }

    this->daytime = is_daytime;
}

void Map::switchViewmode() {
    if (this->viewmode == VIEWMODE_TOP) {
        this->viewmode = VIEWMODE_SIDE;
    } else {
        this->viewmode = VIEWMODE_TOP;
    }

    this->isSwitchingViewmode = true;
}

void Map::update(float time) {
    if (!this->isScaling) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            if (GRID_WIDTH < MAP_WIDTH && GRID_HEIGHT < MAP_HEIGHT) {
                ACTUAL_GRID_WIDTH += 1;
                ACTUAL_GRID_HEIGHT += ASPECT_RATIO;

                GRID_WIDTH = int(ACTUAL_GRID_WIDTH);
                GRID_HEIGHT = int(ACTUAL_GRID_HEIGHT);
                isScaling = true;
            }
        } if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
            if (GRID_WIDTH > 3  && GRID_HEIGHT > 3) {
                ACTUAL_GRID_WIDTH -= 1;
                ACTUAL_GRID_HEIGHT -= ASPECT_RATIO;

                GRID_WIDTH = int(ACTUAL_GRID_WIDTH);
                GRID_HEIGHT = int(ACTUAL_GRID_HEIGHT);
                isScaling = true;
            }
        }
    }

    if (this->isScaling) {
        int sign = (this->gridWidth > GRID_WIDTH || this->gridHeight > GRID_HEIGHT) ? -1 : 1;

        this->gridWidth += sign * SCALE_SPEED;
        this->gridHeight += sign * SCALE_SPEED;

        if ((sign < 0 && this->gridWidth < GRID_WIDTH) || (sign > 0 && this->gridWidth > GRID_WIDTH)) {
            this->gridWidth = GRID_WIDTH;
        } if ((sign < 0 && this->gridHeight < GRID_HEIGHT) || (sign > 0 && this->gridHeight > GRID_HEIGHT)) {
            this->gridHeight = GRID_HEIGHT;
        }

        CELL_HEIGHT = GROUND_HEIGHT / gridHeight;
        CELL_WIDTH = WINDOW_WIDTH / gridWidth;

        PLAYER_WIDTH = CELL_WIDTH * PLAYER_WIDTH_COEF;
        PLAYER_HEIGHT = CELL_HEIGHT * PLAYER_HEIGHT_COEF;

        PROP_WIDTH = CELL_WIDTH * 0.2;
        PROP_HEIGHT = PROP_WIDTH;

        if (this->gridWidth == GRID_WIDTH && this->gridHeight == GRID_HEIGHT) {
            isScaling = false;
        }
    }

    /*
    if (!this->isSwitchingViewmode && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        this->switchViewmode();
    }
    */

    if (this->isSwitchingViewmode) {
        if (this->viewmode == VIEWMODE_TOP) {
            GRID_HEIGHT += 1;
            SKY_HEIGHT -= VIEWMODE_SWITCH_SPEED;
            PLAYER_HEIGHT -= 1;
            if (SKY_HEIGHT < MIN_SKY_HEIGHT) {
                SKY_HEIGHT = MIN_SKY_HEIGHT;
                isSwitchingViewmode = false;
            }
        } else {
            GRID_HEIGHT -= 1;
            SKY_HEIGHT += VIEWMODE_SWITCH_SPEED;
            PLAYER_HEIGHT += 1;
            if (SKY_HEIGHT > MAX_SKY_HEIGHT) {
                SKY_HEIGHT = MAX_SKY_HEIGHT;
                isSwitchingViewmode = false;
            }
        }

        GROUND_HEIGHT = int(WINDOW_HEIGHT - SKY_HEIGHT);
        CELL_HEIGHT = GROUND_HEIGHT / GRID_HEIGHT;
//            PLAYER_HEIGHT = CELL_HEIGHT / 2;
    }

    this->tick();
}

void Map::draw(float px, float py, int dx, int dy) {
    if (this->viewmode == VIEWMODE_SIDE || isSwitchingViewmode) {
        sf::RectangleShape sky_rect;

        sky_rect.setSize(sf::Vector2f(WINDOW_WIDTH, SKY_HEIGHT));
        sky_rect.setPosition(0, 0);
        sky_rect.setFillColor(SKY);

        this->window->draw(sky_rect);
    }

    if (isSwitchingViewmode) {
        sf::RectangleShape ground_rect;

        ground_rect.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - SKY_HEIGHT));
        ground_rect.setPosition(0, SKY_HEIGHT);
        ground_rect.setFillColor(SNOW1);

        this->window->draw(ground_rect);
    }

    sf::RectangleShape cell_rect, trace_rect, light_rect;
    sf::Color color;
    Cell* cell;

    trace_rect.setTexture(&TEXTURE_TRACE);
    trace_rect.setFillColor(SHADOW);
    trace_rect.setSize(sf::Vector2f(CELL_WIDTH, CELL_HEIGHT));
    trace_rect.setOrigin(CELL_WIDTH / 2, CELL_HEIGHT / 2);

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

    int left_solid = int(left) == left ? int(left) : int(left) + 1;
    int top_solid = int(top) == top ? int(top) : int(top) + 1;
    int right_solid = int(right);
    int bottom_solid = int(bottom);

    float left_frac = left_solid - left;
    float top_frac = top_solid - top;
    float right_frac = right - right_solid;
    float bottom_frac = bottom - bottom_solid;

    int left_edge = left_frac == 0 ? left : left_solid - 1;
    int top_edge = top_frac == 0 ? top : top_solid - 1;
    int right_edge = right_frac == 0 ? right : right_solid + 1;
    int bottom_edge = bottom_frac == 0 ? bottom : bottom_solid + 1;

    if (left_edge < 0) {
        left_edge = 0;
    } if (top_edge < 0) {
        top_edge = 0;
    } if (right_edge > MAP_WIDTH - 1) {
        right_edge = MAP_WIDTH - 1;
    } if (bottom_edge > MAP_HEIGHT - 1) {
        bottom_edge = MAP_HEIGHT - 1;
    }

    float cell_x, cell_y;
    float cell_width, cell_height;

    // drawing map cells

    for (int i = left_edge; i < right_edge + 1; i++) {
        for (int j = top_edge; j < bottom_edge + 1; j++) {
            
            cell = this->get(i, j);

            if (!cell->fog) {
                color = cell->color;
            } else {
                color = FOG;
            }

            cell_rect.setFillColor(color);

            cell_x = (left_frac + i - left_solid) * CELL_WIDTH;
            cell_y = SKY_HEIGHT + (top_frac + j - top_solid) * CELL_HEIGHT;

            cell_width = CELL_WIDTH;
            cell_height = CELL_HEIGHT;

            if (i == left_edge && left_frac != 0) {
                cell_width = CELL_WIDTH * left_frac;
                cell_x = 0;
            } else if (i == right_edge && right_frac != 0) {
                cell_width = CELL_WIDTH * right_frac;
                cell_x = CELL_WIDTH * left_frac + (right_solid - left_solid + 1) * CELL_WIDTH;
            }
            
            if (j == top_edge && top_frac != 0) {
                cell_height = CELL_HEIGHT * top_frac;
                cell_y = 0;
            } else if (j == bottom_edge && bottom_frac != 0) {
                cell_height = CELL_HEIGHT * bottom_frac;
                cell_y = CELL_HEIGHT * top_frac + (bottom_solid - top_solid + 1) * CELL_HEIGHT;
            }

            cell_rect.setPosition(cell_x, cell_y);
            cell_rect.setSize(sf::Vector2f(cell_width, cell_height));

            this->window->draw(cell_rect);

            if (!cell->fog && cell->trace_direction != NONE) {
                trace_rect.setRotation(cell->trace_direction);
                trace_rect.setPosition(cell_x + cell_width / 2, cell_y + cell_height / 2);
                this->window->draw(trace_rect);
            }
        }
    }

    light_rect.setFillColor(this->light_color);
    light_rect.setPosition(0, 0);
    light_rect.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    this->window->draw(light_rect);

    // drawing player glow

    this->player->draw_glow();

    // drawing props

    sf::RectangleShape shadow_rect;

    float shadow_length = 16 * CELL_HEIGHT * (dist(this->hour + float(this->minutes) / HOUR_LENGTH, 12) / 12) + CELL_HEIGHT;
    shadow_rect.setTexture(&TEXTURE_SHADOW);
    shadow_rect.setFillColor(SHADOW);
    shadow_rect.setSize(sf::Vector2f(PROP_WIDTH, shadow_length));
    shadow_rect.setOrigin(sf::Vector2f(PROP_WIDTH / 2, shadow_length));
    shadow_rect.setRotation((this->hour + float(this->minutes) / HOUR_LENGTH - 12) * 15);

    for (int i = left_solid; i < right_solid + 1; i++) {
        for (int j = top_solid; j < bottom_solid + 1; j++) {
            cell = this->get(i, j);
            cell_rect.setFillColor(color);

            cell_x = (left_frac + i - left_solid) * CELL_WIDTH;
            cell_y = SKY_HEIGHT + (top_frac + j - top_solid) * CELL_HEIGHT;

            cell_width = CELL_WIDTH;
            cell_height = CELL_HEIGHT;

            if (i == left_edge && left_frac != 0) {
                cell_width = CELL_WIDTH * left_frac;
                cell_x = 0;
            } else if (i == right_edge && right_frac != 0) {
                cell_width = CELL_WIDTH * right_frac;
                cell_x = CELL_WIDTH * left_frac + (right_solid - left_solid + 1) * CELL_WIDTH;
            }
            
            if (j == top_edge && top_frac != 0) {
                cell_height = CELL_HEIGHT * top_frac;
                cell_y = 0;
            } else if (j == bottom_edge && bottom_frac != 0) {
                cell_height = CELL_HEIGHT * bottom_frac;
                cell_y = CELL_HEIGHT * top_frac + (bottom_solid - top_solid + 1) * CELL_HEIGHT;
            }

            if (cell->has_prop && !cell->fog) {
                Prop* prop = cell->prop;
                prop->draw(cell_x, cell_y);

                if (prop->type == PROP_OBSTACLE) {
                    shadow_rect.setPosition(cell_x + CELL_WIDTH / 2, cell_y + CELL_HEIGHT / 2);
                    this->window->draw(shadow_rect);
                }
            }
        }
    }

}
