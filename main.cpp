#include <iostream>
#include <ctime>

#include "sdl_init.h"
#include "Painter.h"
#include "init.h"
#include "game/game.h"

using namespace std;

void renderSplashScreen() {
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)) return;
        SDL_Delay(100);
    }
}

int main(int agrc, char **argv)
{
    srand(time(NULL));

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    initSDL(window, renderer);

    Painter painter(window, renderer);
    Game game(renderer, &event, &painter, GAME_WIDTH, GAME_HEIGHT);

    renderSplashScreen();
    game.init();
    while (game.isGameRunning()) {
        game.handleEvent();
        game.process();
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    waitUntilKeyPressed();
    quitSDL(window, renderer);

    return 0;
}
