#include "game.h"

Game::Game(SDL_Renderer *_renderer, SDL_Event *_event, Painter *_painter, int _width, int _height):
    renderer(_renderer), event(_event), painter(_painter),
    width(_width), height(_height),
    background(BACKGROUND, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT})
{
    gallery = new Gallery(painter);
    setGameStatus(GAME_RUNNING);
    score = 0;
    round = 0;
    roundWon = true;

    enemyMoveState = {0, 1, 0, 0};

    background.setTexture(gallery->background);
    gundam.getEntity()->setTexture(gallery->gundam);
}

void Game::setGameStatus(GameStatus newStatus) {
    status = newStatus;
//    cout << newStatus << " " << getGameStatus() << "\n";
}


void Game::init() {
    roundWon = false;
    chickenBullets.clear();
    chickens.clear();
    topChicken = bottomChicken = leftChicken = rightChicken = NULL;

    int level = (round == BOSS_ROUND || round == MINI_BOSS_ROUND);
    int perRow = round == BOSS_ROUND ? 3 : (round == MINI_BOSS_ROUND ? 1 : NUMBER_OF_CHICKEN_PER_ROW);
    int numberOfEnemy = (round == BOSS_ROUND || round == MINI_BOSS_ROUND) ? perRow : NUMBER_OF_CHICKEN;
    int numberOfBullet = perRow;

    enemyMoveState = {0, 1, 0, !level};

    for (int i = 0; i < numberOfBullet; ++ i) chickenBullets.push_back(new Bullet());
    for (int i = 0; i < numberOfEnemy; ++ i) {
        int row = i / perRow, col = i % perRow;
        Chicken *chicken = new Chicken(col, row, level);
        chicken->getEntity()->setTexture(gallery->chickens[chicken->getLevel()]);
        chickens.insert(chicken);
    }

//    cout << "Init Successful\n";
}

void Game::process() {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            setGameStatus(GAME_STOP);
            return;
        }
        if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
            gundam.control(*event, gallery);
        }
    }

    painter->clearWithBgColor(WHITE_COLOR);


    background.render(renderer);

    gundam._move();
//    cout << (gallery->gundam).w << " " << (gallery->gundam).h << "\n";
    gundam.render(renderer);
    gundam.handleBullet(renderer);

    if (roundWon) {
        initEnd = CLOCK_NOW();
        ElapsedTime elapsed = initEnd - initStart;
        if (elapsed.count() > INIT_DELAY) {
            if (round == BOSS_ROUND) {
                setGameStatus(GAME_WON);
                return;
            }
//            cerr << "Start init\n";
            round ++;
            init();
        }
    }

    int chicken_step_x = 0, chicken_step_y = 0;

    if (!chickens.empty()) {
        topChicken = leftChicken = *chickens.begin();
        bottomChicken = rightChicken = *chickens.rbegin();
        for (Chicken *chicken: chickens) {
            if (chicken->getEntity()->getX() < leftChicken->getEntity()->getX()) leftChicken = chicken;
            if (chicken->getEntity()->getX() > rightChicken->getEntity()->getX()) rightChicken = chicken;
            if (chicken->getEntity()->getY() < topChicken->getEntity()->getY()) topChicken = chicken;
            if (chicken->getEntity()->getY() > bottomChicken->getEntity()->getY()) bottomChicken = chicken;
        }
    }
    else {
        topChicken = bottomChicken = leftChicken = rightChicken = NULL;
    }

    if (topChicken != nullptr) {
        if (enemyMoveState.goLeft) chicken_step_x = -topChicken->getSpeed();
        if (enemyMoveState.goRight) chicken_step_x = topChicken->getSpeed();
        if (enemyMoveState.goUp) chicken_step_y = -topChicken->getSpeed();
        if (enemyMoveState.goDown) chicken_step_y = topChicken->getSpeed();
    }

    for (Chicken *chicken: chickens) {
        chicken->_move(chicken_step_x, chicken_step_y);

        const int rate = chicken->getLevel() == 0 ? 1 : 100;
        const int maxNumberOfBullet = chicken->getLevel() == 0 ? 1 : 3;
        auto _now = CLOCK_NOW();
        ElapsedTime elapsed = _now - chicken->getLastBullet();
        if (rand() % 1000 < rate && chicken->getNumberOfBullet() < maxNumberOfBullet && !chickenBullets.empty() && elapsed.count() > BULLET_DELAY) {
            chicken->addBullet(chickenBullets.back());
            chickenBullets.pop_back();
            chicken->setLastBullet(_now);
        }
        chicken->render(renderer);
        chicken->handleBullet(renderer, chickenBullets);
    }

    if (topChicken != nullptr) {
        if (bottomChicken->getEntity()->getY() > bottomChicken->getEntity()->getH() * 5) {
            enemyMoveState.goDown = 0;
            enemyMoveState.goUp = 1;
        }
        if (topChicken->getEntity()->getY() < 0) {
            enemyMoveState.goDown = 1;
            enemyMoveState.goUp = 0;
        }
        if (rightChicken->getEntity()->getX() > SCREEN_WIDTH - rightChicken->getEntity()->getW()) {
            enemyMoveState.goRight = 0;
            enemyMoveState.goLeft = 1;
        }
        if (leftChicken->getEntity()->getX() < 0) {
            enemyMoveState.goRight = 1;
            enemyMoveState.goLeft = 0;
        }
    }

    handleGameEvent();

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}

void Game::handleGameEvent() {
    for (Chicken *chicken: chickens) {
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
//                        cerr << "Round won\n";
                        return;
                    }
                    break;
                }
            }
        }
    }
}
