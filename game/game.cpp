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

    chickenMoveState = {0, 1, 0, 0};

    background.setTexture(gallery->background);
    gundam.getEntity()->setTexture(gallery->gundams[GUNDAM_BLASTER]);

    killedChickenCount.assign(5, 0);
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
    numberOfAliveChicken = 0;

    int level = (round == BOSS_ROUND || round == MINI_BOSS_ROUND);
    int perRow = round == BOSS_ROUND ? 3 : (round == MINI_BOSS_ROUND ? 1 : NUMBER_OF_CHICKEN_PER_ROW);
    int numberOfEnemy = (round == BOSS_ROUND || round == MINI_BOSS_ROUND) ? perRow : NUMBER_OF_CHICKEN;
    int numberOfBullet = (level ? perRow * 2 : perRow);

    chickenMoveState = {0, 1, 0, !level};

    for (int i = 0; i < numberOfBullet; ++ i) chickenBullets.push_back(new Bullet());
    for (int i = 0; i < numberOfEnemy; ++ i) {
        int row = i / perRow, col = i % perRow;
        Chicken *chicken = new Chicken(col, row, level);
        chicken->getEntity()->setTexture(gallery->chickens[chicken->getLevel()]);
        chickens.push_back(chicken);
    }
    numberOfAliveChicken = numberOfEnemy;

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


    (scrolling += SCREEN_SPEED) %= BG_SIZE;
    background.render(renderer, scrolling);

    // ............................gundam.......................................
    gundam._move();
//    cout << (gallery->gundam).w << " " << (gallery->gundam).h << "\n";
    gundam.render(renderer, gallery);
    gundam.handleBullet(renderer);

    //..............................round...........................................
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

    //................................chicken.........................................
    int chicken_step_x = 0, chicken_step_y = 0;
    topChicken = bottomChicken = leftChicken = rightChicken = NULL;

    if (!chickens.empty()) {
//        topChicken = leftChicken = *chickens.begin();
//        bottomChicken = rightChicken = *chickens.rbegin();
        for (Chicken *chicken: chickens) if (chicken->isAlive()) {
            if (topChicken == nullptr) {
                topChicken = bottomChicken = leftChicken = rightChicken = chicken;
                continue;
            }
            if (chicken->getEntity()->getX() < leftChicken->getEntity()->getX()) leftChicken = chicken;
            if (chicken->getEntity()->getX() > rightChicken->getEntity()->getX()) rightChicken = chicken;
            if (chicken->getEntity()->getY() < topChicken->getEntity()->getY()) topChicken = chicken;
            if (chicken->getEntity()->getY() > bottomChicken->getEntity()->getY()) bottomChicken = chicken;
        }
    }

    if (topChicken != nullptr) {
        if (chickenMoveState.goLeft) chicken_step_x = -topChicken->getSpeed();
        if (chickenMoveState.goRight) chicken_step_x = topChicken->getSpeed();
        if (chickenMoveState.goUp) chicken_step_y = -topChicken->getSpeed();
        if (chickenMoveState.goDown) chicken_step_y = topChicken->getSpeed();
    }

    for (Chicken *chicken: chickens) {
        if (chicken->isAlive()) {
            chicken->_move(chicken_step_x, chicken_step_y);

            const int rate = chicken->getLevel() == 0 ? 1 : 100;
            const int maxNumberOfBullet = chicken->getLevel() == 0 ? 1 : 3;
            auto _now = CLOCK_NOW();
            ElapsedTime elapsed = _now - chicken->getLastBullet();
            if (rand() % 1000 < rate && chicken->getNumberOfBullet() < maxNumberOfBullet && !chickenBullets.empty() && elapsed.count() > BULLET_DELAY) {
                chicken->addBullet(chickenBullets.back(), gallery);
                chickenBullets.pop_back();
                chicken->setLastBullet(_now);
            }
            chicken->render(renderer);
        }
        chicken->handleBullet(renderer, chickenBullets);
    }

    if (topChicken != nullptr) {
        if (bottomChicken->getEntity()->getY() > bottomChicken->getEntity()->getH() * 5) {
            chickenMoveState.goDown = 0;
            chickenMoveState.goUp = 1;
        }
        if (topChicken->getEntity()->getY() < 0) {
            chickenMoveState.goDown = 1;
            chickenMoveState.goUp = 0;
        }
        if (rightChicken->getEntity()->getX() > SCREEN_WIDTH - rightChicken->getEntity()->getW()) {
            chickenMoveState.goRight = 0;
            chickenMoveState.goLeft = 1;
        }
        if (leftChicken->getEntity()->getX() < 0) {
            chickenMoveState.goRight = 1;
            chickenMoveState.goLeft = 0;
        }
    }

    //................................upgrade................................................................
    for (auto *upgrade: upgrades) {
        upgrade->_move();
        upgrade->render(renderer);
        if (!upgrade->isInsideScreen()) upgrades.erase(upgrade);
    }

    //.............................explosion..................................................................
    while (!explosions.empty() && explosions.front()->getFrame() == NUMBER_OF_EXPLOSION_PIC * FRAME_PER_PICTURE - 1) explosions.pop_front();
    for (auto *explosion: explosions) explosion->render(renderer);

    handleGameEvent();

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}

void Game::dropUpgrade(EntityType eType) {
    if (eType != LEVEL_UP && eType != NEW_WEAPON) return;

    int x = (1ll * rand() * rand()) % SCREEN_WIDTH, y = -10;
    UpgradeType uType = (eType == LEVEL_UP ? UPGRADE_LEVEL_UP : UpgradeType(rand() % 3));
    Upgrade *upgrade = new Upgrade(uType, {x, y, 0, 0});
    upgrade->getEntity()->setStep(0, UPGRADE_SPEED);
    if (uType == UPGRADE_LEVEL_UP) {
        upgrade->getEntity()->setTexture(gallery->levelUp);
    }
    else {
        upgrade->getEntity()->setTexture(gallery->newWeapons[uType]);
    }
    upgrades.insert(upgrade);
}

void Game::handleGameEvent() {
    for (Upgrade *upgrade: upgrades) {
        if (gundam.isAlive() && upgrade->getEntity()->collisionWith(*gundam.getEntity())) {
//            cout << "Level Up\n";
            if (upgrade->getType() == UPGRADE_LEVEL_UP) {
                gundam.levelUp();
            }
            else {
//                cout << upgrade->getType() << "\n";
                gundam.addWeapon(WeaponType(upgrade->getType()));
            }
            upgrades.erase(upgrade);

        }
    }

    for (Chicken *chicken: chickens) {
        if (chicken->isAlive()) {
            if (chicken->getEntity()->collisionWith(*gundam.getEntity())) {
                if (gundam.isAlive()) {
//                    gundam.dead();
                    gundamDead();
                }
            }
        }

        set<Bullet*> currentChickenBullets = chicken->getBullets();
        if (gundam.isAlive()) {
            for (Bullet *bullet: currentChickenBullets) {
                if (bullet->getEntity()->collisionWith(*gundam.getEntity())) {
                    chickenBullets.push_back(bullet);
                    chicken->removeBullet(bullet);
//                    gundam.dead();
                    gundamDead();
                    break;
                }
            }
        }

        if (chicken->isAlive()) {
            set<Bullet*> gundamBullets = gundam.getBullets();
            for (Bullet *bullet: gundamBullets) {
                if (chicken->getEntity()->collisionWith(*(bullet->getEntity()))) {
                    bool alive = chicken->receiveDamage(gundam.getBulletDamage());
                    gundam.removeBullet(bullet);
                    if (!alive) {
    //                    chickens.erase(chicken);

                        addExplosion(chicken->getEntity()->getRect());
                        int chickenLevel = chicken->getLevel();
                        int killed = (++ killedChickenCount[chickenLevel]);
                        if (chickenLevel == 0 && killed % 15 == 0) {
                            dropUpgrade(LEVEL_UP);
                        }
                        if (chickenLevel == 1) {
                            dropUpgrade(NEW_WEAPON);
                        }

                        numberOfAliveChicken --;
                        if (numberOfAliveChicken == 0) {
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
}

void Game::addExplosion(SDL_Rect rect) {
    Texture texture = gallery->expolosion;
//    int w = texture.w / NUMBER_OF_EXPLOSION_PIC, h = texture.h;
//    int center_x = (rect.x + rect.w/2), center_y = (rect.y + rect.h/2);
//    cout << center_x - w/2 << " " << center_y - h/2 << "\n";
//    explosions.push_back(new Entity(EXPLOSION, {center_x - w/2, center_y - h/2, w, h}, texture));
    explosions.push_back(new Entity(EXPLOSION, rect, texture));
}

void Game::gundamDead() {
    gundam.dead();
    addExplosion(gundam.getEntity()->getRect());
}
