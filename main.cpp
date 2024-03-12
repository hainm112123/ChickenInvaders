#include <iostream>
#include <ctime>

#include "sdl_init.h"
#include "Painter.h"
#include "common/common.h"
#include "game/game.h"
#include "time.h"

using namespace std;

int main(int agrc, char **argv)
{
    srand(time(NULL));

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    initSDL(window, renderer);

    Painter painter(window, renderer);
    Game game(renderer, &event, &painter, GAME_WIDTH, GAME_HEIGHT);


    auto a = SDL_GetTicks();
    auto b = SDL_GetTicks();

    game.load();
    while (game.isRunning()) {
        a = SDL_GetTicks();
        double delta = a - b;
        if (delta > (double)(1000)/144) {
            TimeManager::Instance()->process();
//            cout << "FPS: " << 1000 / delta << "\n";
            b = a;
            game.renderMenu();
            game.process();
            game.gameOver();
        }
    }

    game.quit();
    quitSDL(window, renderer);

    return 0;
}
