#include "stdafx.h"
#include "map.cpp"
#include "player.cpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Woods");
    sf::Clock clock;

    TEXTURE_AGENT.loadFromFile("sprites/agent.png");
    TEXTURE_PLAYER.loadFromFile("sprites/player.png");
    TEXTURE_ENEMY.loadFromFile("sprites/enemy.png");
    TEXTURE_GOLD.loadFromFile("sprites/gold.png");
    TEXTURE_SHADOW.loadFromFile("sprites/shadow.png");
    TEXTURE_TRACE.loadFromFile("sprites/trace.png");
    TEXTURE_GLOW.loadFromFile("sprites/glow.png");

    srand (time(NULL));

    Map map(&window);
    Player player(&window, &map);

    Agent enemy(&window, &map);

    float runtime;

    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                    (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }
        runtime = clock.getElapsedTime().asMilliseconds();

        if (runtime > (1000 / FPS)) {
            clock.restart();
        } else {
            continue;
        }

        window.clear();

        map.update(runtime);
        player.update(runtime);
        enemy.update(runtime);

        map.draw(player.X, player.Y, player.dest_x, player.dest_y);
        player.draw_player();

        enemy.draw(player.X, player.Y);

        window.display();
    }
    return 0;
}
