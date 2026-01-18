#include "Game.h"
#include <SDL2/SDL_image.h>
#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), running(false) {}

Game::~Game() { cleanup(); }

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("BeatEmUp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << "\n";
        return false;
    }

    stage.loadStage("stage1.txt");
    player.init(renderer);

    enemies.push_back(Enemy(400, 300, Aggressive));
    enemies.push_back(Enemy(600, 300, Ranged));

    running = true;
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    while (running) {
        Uint32 current = SDL_GetTicks();
        float dt = (current - lastTime) / 1000.0f;
        lastTime = current;

        handleEvents();
        update(dt);
        render();
    }
}

void Game::cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
    }
    handleInput(1.0f / 60.0f); // przykładowe dt
}

void Game::handleInput(float dt) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_LEFT]) pushInput(&combo, LEFT, SDL_GetTicks()/1000.0f);
    if (state[SDL_SCANCODE_RIGHT]) pushInput(&combo, RIGHT, SDL_GetTicks()/1000.0f);
    if (state[SDL_SCANCODE_X]) pushInput(&combo, X, SDL_GetTicks()/1000.0f);
    if (state[SDL_SCANCODE_Y]) pushInput(&combo, Y, SDL_GetTicks()/1000.0f);

    player.processCombo(combo);
}

void Game::update(float dt) {
    player.update(dt);
    for (auto& enemy : enemies) {
        enemy.update(dt, player);
        if (player.checkHit(enemy)) {
            player.onHit(enemy.getDamage());
            enemy.stun();
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    stage.render(renderer);
    player.render(renderer);
    for (auto& enemy : enemies) enemy.render(renderer);

    SDL_RenderPresent(renderer);
}
