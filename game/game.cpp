#include "game.h"

Game::Game(SDL_Renderer *_renderer, SDL_Event *_event, Painter *_painter, int _width, int _height):
    renderer(_renderer), event(_event), painter(_painter), width(_width), height(_height)
{

}

void Game::setGameStatus(GameStatus newStatus) {
    status = newStatus;
}

void Game::init() {
    setGameStatus(GAME_RUNNING);
    chickenBullets.clear();
    for (int i = 0; i < NUMBER_OF_CHICKEN_PER_ROW; ++ i) chickenBullets.push_back(new Bullet());
    for (int i = 0; i < NUMBER_OF_CHICKEN; ++ i) {
        int row = i / NUMBER_OF_CHICKEN_PER_ROW, col = i % NUMBER_OF_CHICKEN_PER_ROW;
        Chicken *chicken = new Chicken(col * (CHICKEN_WIDTH + CHICKENS_DISTANCE), row * (CHICKEN_HEIGHT + CHICKENS_DISTANCE));
        chickens.insert(chicken);
    }
}

void Game::handleEvent() {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            setGameStatus(GAME_STOP);
            return;
        }
        if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
            gundam.control(*event);
        }
    }
}

void Game::process() {
    painter->clearWithBgColor(BLACK_COLOR);

    gundam._move();
    gundam.render(renderer, painter);
    gundam.handleBullet(renderer);

    handleChicken();
}

void Game::handleChicken() {
    int cnt = 0;
    for (Chicken *chicken: chickens) {
        chicken->_move();
        cnt ++;
        if (rand() % 100 < 15 && chicken->getNumberOfBullet() < 1 && !chickenBullets.empty()) {
            chicken->addBullet(chickenBullets.back());
            chickenBullets.pop_back();
        }
        chicken->render(renderer);
        chicken->handleBullet(renderer, chickenBullets);

        if (chicken->getEntity()->collisionWith(*gundam.getEntity())) {
            if (gundam.isAlive()) gundam.dead();
        }

        set<Bullet*> currentChickenBullets = chicken->getBullets();
        if (gundam.isAlive()) {
            for (Bullet *bullet: currentChickenBullets) {
                if (bullet->getEntity()->collisionWith(*gundam.getEntity())) {
                    chickenBullets.push_back(bullet);
                    chicken->removeBullet(bullet);
                    gundam.dead();
                    break;
                }
            }
        }

        set<Bullet*> gundamBullets = gundam.getBullets();
        for (Bullet *bullet: gundamBullets) {
            if (chicken->getEntity()->collisionWith(*(bullet->getEntity()))) {
                bool alive = chicken->receiveDamage(gundam.getBulletDamage());
                gundam.removeBullet(bullet);
                if (!alive) {
                    chickens.erase(chicken);
                    break;
                }
            }
        }
    }
}
