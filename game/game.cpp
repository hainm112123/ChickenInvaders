#include "game.h"

Game::Game(SDL_Renderer *_renderer, SDL_Event *_event, Painter *_painter, int _width, int _height):
    renderer(_renderer), event(_event), painter(_painter),
    width(_width), height(_height),
    background(BACKGROUND, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT})
{
    media = new Media();
    gallery = new Gallery(painter);
    setGameStatus(GAME_STOP);
    score = 0;
    round = 0;
    roundWon = true;

    chickenMoveState = {0, 1, 0, 0};

    background.setTexture(gallery->background);
    gundam.getEntity()->setTexture(gallery->gundams[GUNDAM_BLASTER]);

    killedChickenCount.assign(5, 0);
}
Game::~Game() {
    quit();
}
void Game::setGameStatus(GameStatus newStatus) {
    status = newStatus;
//    cout << newStatus << " " << getGameStatus() << "\n";
}


void Game::init() {
//    cout << round << "\n";
    roundWon = false;
    chickenBullets.clear();
    chickens.clear();
    topChicken = bottomChicken = leftChicken = rightChicken = NULL;
    numberOfAliveChicken = 0;
    frame = 0;

    if (round == ROCK_SIDE_ROUND) {

    }
    else if (round == ROCK_FALL_ROUND) {

    }
    else {
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
            gundam.control(*event, gallery, media);
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

    //...............................rock..............................................
    int rockWaveCount = round == ROCK_FALL_ROUND ? ROCK_FALL_WAVE : ROCK_SIDE_WAVE;
    if ((round == ROCK_FALL_ROUND || round == ROCK_SIDE_ROUND) && frame <= (rockWaveCount - 1) * ROCK_WAVE_FRAME) {
        if (frame % ROCK_WAVE_FRAME == 0) {
            int n = 5;
            int L = (round == ROCK_FALL_ROUND ? (SCREEN_WIDTH / 5) * 4 : SCREEN_HEIGHT) / n;
            for (int i = 0; i < n; ++ i) if (Rand(0, 10) < 8) {
                int x, y, step_x, step_y;
                if (round == ROCK_FALL_ROUND) {
                    x = Rand(L * i + 10, L * (i + 1) - 10) ;
                    y = -50;
                    step_x = 0;
                    step_y = Rand(MIN_ROCK_FALL_SPEED, MAX_ROCK_FALL_SPEED);
                }
                else {
                    x = -50;
                    y = Rand(L * i + 10, L * (i + 1) - 10);
                    step_x = Rand(MIN_ROCK_SIDE_SPEED_X, MAX_ROCK_SIDE_SPEED_X);
                    step_y = Rand(MIN_ROCK_SIDE_SPEED_Y, MAX_ROCK_SIDE_SPEED_Y);
                }
                int _size = Rand(MIN_ROCK_SIZE, MAX_ROCK_SIZE);
                Rock *rock = new Rock(ROCK, {x, y, _size, _size}, gallery->rocks[Rand(0, _size(gallery->rocks) - 1)]);
                rock->setStep(step_x, step_y);
                rock->setActive(true);
                rocks.insert(rock);
//                cout << "New Rock" << rock->getX() << " " << rock->getY() << " " << rock->getW() << " " << rock->getH() << "\n";
            }
        }
        frame ++;
//        cout << frame << "\n";
    }

    for (Rock *rock: rocks) {
        if (rock->isActive()) {
            rock->handleMove();
            rock->render(renderer);
        }
        else {
            rocks.erase(rock);
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

void Game::setRoundWon() {
    cout << "Round Won\n";
    roundWon = true;
    initStart = CLOCK_NOW();
}

void Game::handleGameEvent() {
    //.............................upgrade.....................................................
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
            Mix_PlayChannel(-1, media->upgrade, 0);
        }
    }

    //............................chicken...................................................
    for (Chicken *chicken: chickens) {
        if (chicken->isAlive()) {
            if (chicken->getEntity()->collisionWith(*gundam.getEntity())) {
                if (gundam.isAlive()) {
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
                        Mix_PlayChannel(-1, chicken->getLevel() == 0 ? media->chickens[2] : media->explosions[1], 0);

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
                            setRoundWon();
    //                        cerr << "Round won\n";
                            return;
                        }
                        break;
                    }
                    else {
                        Mix_PlayChannel(-1, media->chickens[Rand(0, 1)], 0);
                    }
                }
            }
        }
    }

    //...............................rock..........................................
    for (Rock *rock: rocks) if (rock->isActive()) {
        if (gundam.isAlive() && rock->collisionWith(*gundam.getEntity())) {
            gundamDead();
        }
        if (gundam.isAlive()) {
            set<Bullet*> gundamBullets = gundam.getBullets();
            for (Bullet *bullet: gundamBullets) {
                if (rock->collisionWith(*(bullet->getEntity()))) {
                    rock->receiveDamage(gundam.getBulletDamage());
                    gundam.removeBullet(bullet);
                    Mix_PlayChannel(-1, media->bulletRock, 0);
                }
            }
        }
    }

    int rockWaveCount = round == ROCK_FALL_ROUND ? ROCK_FALL_WAVE : ROCK_SIDE_WAVE;
    if ((round == ROCK_FALL_ROUND || round == ROCK_SIDE_ROUND) && frame > (rockWaveCount - 1) * ROCK_WAVE_FRAME && rocks.empty() && !roundWon) {
        setRoundWon();
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
    Mix_PlayChannel(-1, media->explosions[0], 0);
}

void Game::renderMenu() {
    Entity menu(MENU, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, gallery->menu);

    vector<Text> texts = {
        Text("Start", WHITE_COLOR),
        Text("Quit", WHITE_COLOR),
    };

    texts[0].renderText(fontMenu, renderer);
    texts[0].setRect(SCREEN_WIDTH/2 - texts[0].getW()/2, SCREEN_HEIGHT - 250);

    texts[1].renderText(fontMenu, renderer);
    texts[1].setRect(SCREEN_WIDTH/2 - texts[1].getW()/2, SCREEN_HEIGHT - 200);

    bool menuRunning = true;
    Mix_PlayMusic(media->music, -1);
    while (menuRunning) {
        menu.render(renderer);
        for (int i = 0; i < int(texts.size()); ++ i) {
            texts[i].renderText(fontMenu, renderer);
        }
        while (SDL_PollEvent(event) != 0) {
            if (event->type == SDL_QUIT) {
                setGameStatus(GAME_STOP);
                menuRunning = false;
            }
            else if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < int(texts.size()); ++ i) {
                    if (texts[i].getX() <= (event->motion).x && (event->motion).x <= texts[i].getX() + texts[i].getW() && texts[i].getY() <= (event->motion).y && (event->motion).y <= texts[i].getY() + texts[i].getH()) {
                        if (event->type == SDL_MOUSEMOTION) {
                            texts[i].setColor(RED_COLOR);
                        }
                        else {
                            Mix_PlayChannel(-1, media->upgrade, 0);
                            if (i == 0) {
                                setGameStatus(GAME_RUNNING);
                                menuRunning = false;
                            }
                            if (i == 1) {
                                setGameStatus(GAME_STOP);
                                menuRunning = false;
                            }
                        }
                    }
                    else {
                        texts[i].setColor(WHITE_COLOR);
                    }
                }
            }
        }

        SDL_RenderPresent(renderer);
    }
    Mix_PauseMusic();
}

void Game::load() {
    TTF_Init();
    fontMenu = TTF_OpenFont("./font/font1.ttf", 50);
    fontGame = TTF_OpenFont("./font/font1.ttf", 24);

    setGameStatus(GAME_RUNNING);
}

void Game::quit() {
    TTF_Quit();
}
