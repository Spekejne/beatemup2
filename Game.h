#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Input.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void cleanup();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Player player;
    Stage stage;
    std::vector<Enemy> enemies;
    ComboBuffer combo;

    void handleEvents();
    void handleInput(float dt);
    void update(float dt);
    void render();
};

#endif
