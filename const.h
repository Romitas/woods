#ifndef CONSTANTS_H
#define CONSTANTS_H

const int FPS = 60;

const int NONE = -1;

const int WINDOW_WIDTH = 1366;
const int WINDOW_HEIGHT = 768;

float ACTUAL_GRID_WIDTH = 40;
float ACTUAL_GRID_HEIGHT = ACTUAL_GRID_WIDTH / (float(WINDOW_WIDTH) / float(WINDOW_HEIGHT));

int GRID_WIDTH = int(ACTUAL_GRID_WIDTH);
int GRID_HEIGHT = int(ACTUAL_GRID_HEIGHT);

const float ASPECT_RATIO = ACTUAL_GRID_HEIGHT / ACTUAL_GRID_WIDTH;

//const int SIDE_GRID_WIDTH = int(GRID_WIDTH)
//const int SIDE_GRID_HEIGHT = int(GRID_HEIGHT / 3)

const int MAP_WIDTH = GRID_WIDTH * 10;
const int MAP_HEIGHT = GRID_HEIGHT * 10;

const int VIEWMODE_TOP  = 0;
const int VIEWMODE_SIDE = 1;

const int VIEWMODE_SWITCH_SPEED = 40;
const float SCALE_SPEED = 0.5;

const int MIN_SKY_HEIGHT = int(WINDOW_HEIGHT * 0.0);
const int MAX_SKY_HEIGHT = int(WINDOW_HEIGHT * 0.75);

const int MIN_GLOW_SIZE = 20;
const int MAX_GLOW_SIZE = 30;

int SKY_HEIGHT = MIN_SKY_HEIGHT;
int GROUND_HEIGHT = int(WINDOW_HEIGHT - SKY_HEIGHT);

float CELL_WIDTH = WINDOW_WIDTH / GRID_WIDTH;
float CELL_HEIGHT = GROUND_HEIGHT / GRID_HEIGHT;

float PROP_WIDTH = CELL_WIDTH * 0.2;
float PROP_HEIGHT = PROP_WIDTH;

const float PLAYER_WIDTH_COEF = 1.0;
const float PLAYER_HEIGHT_COEF = 1.0;

float PLAYER_WIDTH = CELL_WIDTH * PLAYER_WIDTH_COEF;
float PLAYER_HEIGHT = CELL_HEIGHT  * PLAYER_HEIGHT_COEF;

const int VIEW_RADIUS = 20;
const int HOUR_LENGTH = 120;

const int UP            =   0;
const int UPRIGHT       =  45;
const int RIGHT         =  90;
const int DOWNRIGHT     = 135;
const int DOWN          = 180;
const int DOWNLEFT      = 225;
const int LEFT          = 270;
const int UPLEFT        = 315;

const int TERRAIN_GRASS  = 1;
const int TERRAIN_DIRT   = 2;
const int TERRAIN_SNOW   = 3;
const int TERRAIN_SAND   = 4;
const int TERRAIN_WATER  = 5;

const int PROP_NONE         = 1;
const int PROP_OBSTACLE     = 2;
const int PROP_COLLECTIBLE  = 3;

const sf::Color NEUTRAL     (128, 128, 128, 255);
const sf::Color TRANSPARENT (  0,   0,   0,   0);
const sf::Color BLACK       (  0,   0,   0);
const sf::Color WHITE       (255, 255, 255);
const sf::Color GRAY        (100, 100, 100);
const sf::Color DARKGRAY    ( 40,  40,  40);
const sf::Color RED         (255,   0,   0);
const sf::Color GREEN       (  0, 255,   0);
const sf::Color BLUE        (  0,   0, 255);
const sf::Color YELLOW      (255, 255,   0);
const sf::Color ORANGE      (255, 128,   0);
const sf::Color PURPLE      (255,   0, 255);
const sf::Color GOLD        (235, 188,  79);

const sf::Color GRASS1 (50, 150,  5);
const sf::Color GRASS2 (60, 140, 10);
const sf::Color GRASS3 (70, 120, 10);
const sf::Color GRASS4 (40, 130,  5);
const sf::Color GRASS5 (80, 120, 10);

const sf::Color DIRT1 (40, 20, 10);
const sf::Color DIRT2 (45, 15, 15);
const sf::Color DIRT3 (30, 20, 20);
const sf::Color DIRT4 (40, 30, 20);
const sf::Color DIRT5 (30, 25, 15);

const sf::Color SNOW1 (240, 240, 240);
const sf::Color SNOW2 (240, 250, 240);
const sf::Color SNOW3 (240, 240, 250);
const sf::Color SNOW4 (230, 230, 230);

const sf::Color SKY (200, 200, 250);
const sf::Color FOG ( 20,  20,  20);
const sf::Color LIGHT ( 255, 255, 255, 192);
const sf::Color SHADOW ( 0, 0, 0, 32);

const int DAYTIME_MORNING   = 1;
const int DAYTIME_DAY       = 2;
const int DAYTIME_EVENING   = 3;
const int DAYTIME_NIGHT     = 4;

const sf::Color LIGHT_MORNING   (255, 136, 133,  64);
const sf::Color LIGHT_DAY       (255, 225, 192,  64);
const sf::Color LIGHT_EVENING   (255, 128,  55,  64);
const sf::Color LIGHT_NIGHT     (  0,  28,  60, 192);

sf::Texture TEXTURE_AGENT;  
sf::Texture TEXTURE_PLAYER;  
sf::Texture TEXTURE_ENEMY;  
sf::Texture TEXTURE_GOLD;   
sf::Texture TEXTURE_SHADOW; 
sf::Texture TEXTURE_TRACE; 
sf::Texture TEXTURE_GLOW; 

const std::vector<sf::Color> GRASS_COLORS { GRASS1, GRASS2, GRASS3, GRASS4, GRASS5 };
const std::vector<sf::Color> DIRT_COLORS { DIRT1, DIRT2, DIRT3 };
const std::vector<sf::Color> SNOW_COLORS { SNOW1, SNOW2, SNOW3, SNOW4 };

#endif
