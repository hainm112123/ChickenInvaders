#include "game.h"

Game::Game(SDL_Renderer *_renderer, SDL_Event *_event, Painter *_painter, int _width, int _height):
    renderer(_renderer), event(_event), painter(_painter), width(_width), height(_height)
{
    setGameStatus(GAME_RUNNING);
    score = 0;
    round = 0;
    roundWon = true;
}

void Game::setGameStatus(GameStatus newStatus) {
    status = newStatus;
}


void Game::init() {
    roundWon = false;
    chickenBullets.clear();
    chickens.clear();

    int level = (round == BOSS_ROUND || round == MINI_BOSS_ROUND);
    int perRow = round == BOSS_ROUND ? 3 : (round == MINI_BOSS_ROUND ? 1 : NUMBER_OF_CHICKEN_PER_ROW);
    int numberOfEnemy = (round == BOSS_ROUND || round == MINI_BOSS_ROUND) ? perRow : NUMBER_OF_CHICKEN;
    int numberOfBullet = perRow;

    for (int i = 0; i < numberOfBullet; ++ i) chickenBullets.push_back(new Bullet());
    for (int i = 0; i < numberOfEnemy; ++ i) {
        int row = i / perRow, col = i % perRow;
        Chicken *chicken = new Chicken(col, row, level);
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

    if (roundWon) {
        initEnd = CLOCK_NOW();
        chrono::duration<double> elapsed = initEnd - initStart;
        if (elapsed.count() < INIT_DELAY) return;
        if (round == BOSS_ROUND) {
            setGameStatus(GAME_WON);
            return;
        }
        round ++;
        init();
    }

    handleChicken();
}

void Game::handleChicken() {
    for (Chicken *chicken: chickens) {
        chicken->_move();
        if (rand() % 100 < 1 && chicken->getNumberOfBullet() < 1 && !chickenBullets.empty()) {
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
                    if (chickens.empty()) {
                        roundWon = true;
                        initStart = CLOCK_NOW();
                    }
                    break;
                }
            }
        }
    }
}
