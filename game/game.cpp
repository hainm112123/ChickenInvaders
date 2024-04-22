#include "game.h"

bool isHover(const SDL_Event &event, const Entity &entity) {
    return entity.getX() <= event.motion.x && event.motion.x <= entity.getX() + entity.getW() && entity.getY() <= event.motion.y && event.motion.y <= entity.getY() + entity.getH();
}

Game::Game(SDL_Renderer *_renderer, SDL_Event *_event, int _width, int _height):
    renderer(_renderer), event(_event),
    width(_width), height(_height),
    background(BACKGROUND, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}),
    initTimer(INIT_DELAY), gameEndTimer(GAME_END_DELAY), rockWaveTimer(ROCK_WAVE_DELAY), bossTurnTimer(BOSS_TURN_DELAY),
    roundTitle("", TEXT_COLOR), roundText("", TEXT_COLOR),
//    gundam_p1(PLAYER_P1), gundam_p2(PLAYER_P2),
    rocket(SCREEN_WIDTH/2 - ROCKET_WIDTH/2, SCREEN_HEIGHT, SCREEN_WIDTH/2 - ROCKET_WIDTH/2, SCREEN_HEIGHT/2 - ROCKET_HEIGHT/2),
    bossHealthBar(BOSS_HEALTH_BAR), bossHealthBorder(BOSS_HEALTH_BAR),
//    gundamLevelImage(GUNDAM_STATE), rocketMini(GUNDAM_STATE), frychickenMini(GUNDAM_STATE),
    pause_menu(PAUSE_MENU_ELEMENT), home_button(PAUSE_MENU_ELEMENT), audio_button(PAUSE_MENU_ELEMENT), resume_button(PAUSE_MENU_ELEMENT), pause_button(PAUSE_MENU_ELEMENT),
    menu(MENU, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, Gallery::Instance()->menu),
    menu_play(MENU, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, Gallery::Instance()->menu),
    menu_settings(MENU, {0, 0, SCREEN_WIDTH, SCREEN_WIDTH}, Gallery::Instance()->menu_settings),
    menu_control(MENU, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, Gallery::Instance()->menu_control),
    backButton("Back", TEXT_COLOR)
{

    setGameStatus(GAME_STOP);
    difficultyState = GAME_EASY;
    audioState = AUDIO_UNMUTED;
    score = 0;
    round = INIT_ROUND;
    roundWon = true;

    background.setTexture(Gallery::Instance()->background);
//    gundam.getEntity()->setTexture(Gallery::Instance()->gundams[gundam.getCurrentWeapon()]);

    killedChickenCount.assign(5, 0);

    gundam.push_back(Gundam(PLAYER_P1));
    gundam.push_back(Gundam(PLAYER_P2));
    gundam[0].setGame(this);
    gundam[1].setGame(this);
//    gundamLaserTimer.startCountdown();

    bossHealthBorder.setRect({SCREEN_WIDTH/2 - HEALTH_BORDER_WIDTH/2, 5, HEALTH_BORDER_WIDTH, HEALTH_BORDER_HEIGHT});
    bossHealthBar.setRect({bossHealthBorder.getX() + 6, 8, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT});
    bossHealthBar.setTexture(Gallery::Instance()->bossHealthBar);
    bossHealthBorder.setTexture(Gallery::Instance()->bossHealthBorder);

    //.....................pause.............................
    pause_button.setTexture(Gallery::Instance()->pause_button, true);
    pause_button.setRect(SCREEN_WIDTH - pause_button.getW(), 0);

    pause_menu.setTexture(Gallery::Instance()->pause_menu, true);
    pause_menu.setRect(0, 0);
    home_button.setTexture(Gallery::Instance()->home_button, true);
    resume_button.setTexture(Gallery::Instance()->resume_button, true);
    audio_button.setTexture(Gallery::Instance()->audio_unmuted_button, true);
    int button_offset_x = pause_menu.getX() + pause_menu.getW()/2;
    int button_offset_y = pause_menu.getY() + pause_menu.getH()/2 - home_button.getH();
    resume_button.setRect(button_offset_x - resume_button.getW() - 5, button_offset_y);
    home_button.setRect(button_offset_x + 5, button_offset_y);
    audio_button.setRect(button_offset_x - audio_button.getW()/2, pause_menu.getY() + pause_menu.getH()*3/4 - audio_button.getH()/2);

    gundamLevelImage.push_back(Entity(GUNDAM_STATE));
    gundamLevelImage.push_back(Entity(GUNDAM_STATE));
    rocketMini.push_back(Entity(GUNDAM_STATE));
    rocketMini.push_back(Entity(GUNDAM_STATE));
    frychickenMini.push_back(Entity(GUNDAM_STATE));
    frychickenMini.push_back(Entity(GUNDAM_STATE));
    heart.push_back(Entity(HEART));
    heart.push_back(Entity(HEART));
}
Game::~Game() {
    _clear();
    quit();
}
void Game::setGameStatus(GameStatus newStatus) {
    status = newStatus;
    if (status == GAME_INITALIZING) {
        playMusic(Media::Instance()->start);
    }
    else if (status == GAME_PLAYING) {
        playMusic(Media::Instance()->battle);
    }
    else if (status == GAME_OVER) {
        playMusic(Media::Instance()->ending);
    }
//    cout << newStatus << " " << getGameStatus() << "\n";
}

void Game::_clear(bool round_init) {
    for (Chicken *chicken: chickens) delete(chicken);
    chickens.clear();

    for (Bullet *bullet: chickenBullets) delete(bullet);
    chickenBullets.clear();

    for (Rock *rock: rocks) delete(rock);
    rocks.clear();

    if (!round_init) {
        for (FriedChicken *fried_chicken: fried_chickens) delete(fried_chicken);
        fried_chickens.clear();

        for (Upgrade *upgrade: upgrades) delete(upgrade);
        upgrades.clear();

        for (auto explosion: explosions) delete(explosion);
        explosions.clear();
    }
}

void Game::removeUpgrade(Upgrade *upgrade) {
    auto it = upgrades.find(upgrade);
    if (it != upgrades.end()) {
        upgrades.erase(it);
        delete(upgrade);
    }
}
void Game::removeRock(Rock *rock) {
    auto it = rocks.find(rock);
    if (it != rocks.end()) {
        rocks.erase(it);
        delete(rock);
    }
}
void Game::removeFriedChicken(FriedChicken *fried_chicken) {
    delete(fried_chicken);
}

void Game::init() {
    _clear(true);

//    cout << round << "\n";
//    gundamLaserTimer.startCountdown();
    if (round == 1 && NG == 0 && difficultyState == GAME_EASY) {
        for (int i = 0; i < num_players; ++ i) {
            gundam[i].addWeapon(GUNDAM_BORON);
            gundam[i].addWeapon(GUNDAM_NEUTRON);
            gundam[i].addWeapon(GUNDAM_AUTO_AIM);
            gundam[i].changeWeapon();
        }
    }

    roundWon = false;
    chickenBullets.clear();
    chickens.clear();
    topChicken = bottomChicken = leftChicken = rightChicken = NULL;
    numberOfAliveChicken = 0;
    bossHP = 0;
    rockWaveTimer.deactive();

    rocket_damage = ROCKET_DAMAGE + NG_ROCKET_DAMAGE * NG;
    game_difficulty = (NG + int(difficultyState));

    if (round == ROCK_SIDE_ROUND) {
        rockWaveCount = ROCK_SIDE_WAVE + game_difficulty * NG_ROCK_WAVE;
        rockWaveTimer.startCountdown();
    }
    else if (round == ROCK_FALL_ROUND) {
        rockWaveCount = ROCK_FALL_WAVE + game_difficulty * NG_ROCK_WAVE;
        rockWaveTimer.startCountdown();
    }
    else {
        ChickenType chicken_type = CHICKEN_SMALL;
        if (round == MINI_BOSS_ROUND) chicken_type = CHICKEN_DODGE;
        if (round == BOSS_ROUND) chicken_type = CHICKEN_BOSS;
        int perRow = NUMBER_OF_CHICKEN_PER_ROW[round];
        int numberOfEnemy = NUMBER_OF_CHICKEN[round];
        int numberOfBullet = NUMBER_OF_CHICKEN_BULLET[round];
        numberOfBullet *= (game_difficulty * 2 + 1);

        for (int i = 0; i < numberOfBullet; ++ i) chickenBullets.push_back(new Bullet(CHICKEN_EGG));
        for (int i = 0; i < numberOfEnemy; ++ i) {
            int row = i / perRow, col = i % perRow;
            Chicken *chicken = round == CHICKEN_CIRCULAR_ROUND ?
                                new Chicken(this, chicken_type, CHICKEN_CIRCULAR_MOVE, game_difficulty, {CHICKEN_CIRCULAR_DISTANCE_MAX, (360 / numberOfEnemy) * i}) :
                                new Chicken(this, chicken_type, CHICKEN_BASIC_MOVE, game_difficulty, {col, row});
            chicken->getEntity()->setTextures(Gallery::Instance()->chickens[chicken->chicken_type()]);
            chickens.push_back(chicken);
            if (round == BOSS_ROUND || round == MINI_BOSS_ROUND) bossHP += chicken->getHP();
        }
        numberOfAliveChicken = numberOfEnemy;

        if (chicken_type == CHICKEN_BOSS) bossTurnTimer.startCountdown();
    }

    if (round == MINI_BOSS_ROUND) {
        roundTimer.setDuration(MINI_BOSS_TIME_LIMIT);
        roundTimer.startCountdown();
    }

//    gundamLaserTimer.startCountdown();
//    cout << "Init Successful\n";
}

bool touchTop(Chicken *chicken) {
    return chicken->getEntity()->getY() < 0;
}
bool touchBottom(Chicken *chicken, double mul) {
    return (double)(chicken->getEntity()->getY()) > double(chicken->getEntity()->getH()) * mul;
}
bool touchLeft(Chicken *chicken) {
    return chicken->getEntity()->getX() < 0;
}
bool touchRight(Chicken *chicken) {
    return chicken->getEntity()->getX() > SCREEN_WIDTH - chicken->getEntity()->getW();
}

//............................process...............................................
void Game::process_game_state() {
    const int offset = 10;
    const int pad_0 = 5, pad_1 = 10, pad_2 = 15;

//    for (auto &v: hearts) for (auto &heart: v) heart.render(renderer);
    scoreText.renderText(fontGame, renderer);
    scoreValue.setText(to_string(score));
    scoreValue.renderText(fontRoundText, renderer);

    for (int i = 0; i < num_players; ++ i) {
        heartText[i].setText(to_string(gundam[i].getLives()));
        gundamLevelText[i].setText(to_string(gundam[i].getLevel()));
        frychickenText[i].setText(to_string(frychickenCount[i]));
        rocketText[i].setText(to_string(rocketCount[i]));
        heartText[i].renderText(fontGundamState, renderer, true);
        gundamLevelText[i].renderText(fontGundamState, renderer, true);
        frychickenText[i].renderText(fontGundamState, renderer, true);
        rocketText[i].renderText(fontGundamState, renderer, true);

        if (i == 0) {
            heart[i].setRect(pad_1, SCREEN_HEIGHT - heart[i].getH() - pad_0);
            heartText[i].setRect(heart[i].getX() + heart[i].getW() + pad_0, heart[i].getY());
            gundamLevelImage[i].setRect(heartText[i].getX() + heartText[i].getW() + pad_2, heart[i].getY());
            gundamLevelText[i].setRect(gundamLevelImage[i].getX() + gundamLevelImage[i].getW() + pad_0, heart[i].getY());
            frychickenMini[i].setRect(gundamLevelText[i].getX() + gundamLevelText[i].getW() + pad_2, heart[i].getY());
            frychickenText[i].setRect(frychickenMini[i].getX() + frychickenMini[i].getW() + pad_0, heart[i].getY());
            rocketMini[i].setRect(frychickenText[i].getX() + frychickenText[i].getW() + pad_2, heart[i].getY());
            rocketText[i].setRect(rocketMini[i].getX() + rocketMini[i].getW() + pad_0, heart[i].getY());
        }
        else {
            rocketText[i].setRect(SCREEN_WIDTH - pad_1 - rocketText[i].getW(), SCREEN_HEIGHT - heart[i].getH() - pad_0);
            rocketMini[i].setRect(rocketText[i].getX() - rocketMini[i].getW() - pad_0, rocketText[i].getY());
            frychickenText[i].setRect(rocketMini[i].getX() - frychickenText[i].getW() - pad_2, rocketText[i].getY());
            frychickenMini[i].setRect(frychickenText[i].getX() - frychickenMini[i].getW() - pad_0, rocketText[i].getY());
            gundamLevelText[i].setRect(frychickenMini[i].getX() - gundamLevelText[i].getW()- pad_2, rocketText[i].getY());
            gundamLevelImage[i].setRect(gundamLevelText[i].getX() - gundamLevelImage[i].getW() - pad_0, rocketText[i].getY());
            heartText[i].setRect(gundamLevelImage[i].getX() - heartText[i].getW() - pad_2, rocketText[i].getY());
            heart[i].setRect(heartText[i].getX() - heart[i].getW() - pad_0, rocketText[i].getY());
        }

        heart[i].render(renderer);
        gundamLevelImage[i].render(renderer);
        frychickenMini[i].render(renderer);
        rocketMini[i].render(renderer);
        heartText[i].renderText(fontGundamState, renderer);
        gundamLevelText[i].renderText(fontGundamState, renderer);
        frychickenText[i].renderText(fontGundamState, renderer);
        rocketText[i].renderText(fontGundamState, renderer);
    }

    if (round == BOSS_ROUND || round == MINI_BOSS_ROUND) {
        int currentBossHP = 0;
        for (Chicken *chicken: chickens) currentBossHP += chicken->getHP();
        bossHealthBar.render(renderer, 1ll * HEALTH_BAR_WIDTH * currentBossHP / bossHP);
        bossHealthBorder.render(renderer, HEALTH_BORDER_WIDTH);
    }
}

void Game::process_gundam() {
    for (int i = 0; i < num_players; ++ i) {
        if (gundam[i].laserTimer.timeIsUp()) gundam[i].setLaserOn(false);
        if (!gundam[i].isAlive() && gundam[i].reviveTimer.timeIsUp()) {
            if (gundam[i].revive()) gundam[i].shieldTimer.startCountdown();
        }
        gundam[i]._move();
    //    cout << (Gallery::Instance()->gundam).w << " " << (Gallery::Instance()->gundam).h << "\n";
        gundam[i].render(renderer, !gundam[i].shieldTimer.timeIsUp(), !gundam[i].laserTimer.timeIsUp());
    }
}

void Game::process_enemy() {
    static int gundam_ind = Rand(0, num_players - 1);
    static double change_target_time = 0;

    if (round == BOSS_ROUND || round == MINI_BOSS_ROUND) {
        change_target_time += TimeManager::Instance()->getElapsedTime();
    }
    else {
        change_target_time = 0;
    }
    if (change_target_time >= 0.8) {
        change_target_time -= 0.8;
//        gundam_ind = Rand(0, num_players - 1);
        gundam_ind = 1 - gundam_ind;
    }

    enemy_positions.clear();
    for (Rock *rock: rocks) if (!rock->isActive()) {
        removeRock(rock);
    }

    //...............rock.....................................
    for (Rock *rock: rocks) {
        if (rock->isActive()) {
            rock->handleMove();
            rock->render(renderer);
            rock->handleState(renderer);
            enemy_positions.push_back(make_pair(rock->get_act_x(), rock->get_act_y()));
        }
    }

    //.......................chicken......................................
    topChicken = bottomChicken = leftChicken = rightChicken = nullptr;

    if (!chickens.empty()) {
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

    for (Chicken *chicken: chickens) {
        if (chicken->isAlive()) {
            enemy_positions.push_back(make_pair(chicken->getEntity()->get_act_x(), chicken->getEntity()->get_act_y()));
            if (chicken->chicken_type() != CHICKEN_DODGE || chicken->teleportDuration.timeIsUp()) {
                chicken->_move();
            }

            if (chicken->chicken_type() == CHICKEN_BOSS) {
                chicken->useRocket();
//                int gundam_ind = Rand(0, num_players - 1);
                chicken->handleRocket(renderer, gundam[gundam_ind].getEntity()->get_act_x(), gundam[gundam_ind].getEntity()->get_act_y());
                chicken->useLaser();
                chicken->handleLaser(renderer, gundam[gundam_ind].getEntity()->getX());
                if (chicken->UsingLaser()) {
                    chicken->getEntity()->setTextures(Gallery::Instance()->angry_boss);
                }
                else {
                    chicken->getEntity()->setTextures(Gallery::Instance()->chickens[chicken->chicken_type()]);
                }
            }

            int rate = 0;
            if (chicken->chicken_type() == CHICKEN_SMALL) rate = 1;
            if (chicken->chicken_type() == CHICKEN_BOSS) rate = 100;
            int maxNumberOfBullet = 0;
            if (chicken->chicken_type() == CHICKEN_SMALL) maxNumberOfBullet = 1;
            if (chicken->chicken_type() == CHICKEN_BOSS) maxNumberOfBullet = 3;
            if (!chicken->OnRocket() && !chicken->UsingLaser() &&
                rand() % 1000 < rate && chicken->getNumberOfBullet() < maxNumberOfBullet && !chickenBullets.empty() && (chicken->bulletTimer).timeIsUp())
            {
                chicken->addBullet(chickenBullets.back());
                chickenBullets.pop_back();
                (chicken->bulletTimer).startCountdown();
            }

            if (chicken->chicken_type() == CHICKEN_DODGE && !chicken->teleportDuration.timeIsUp()) {
                if (chicken->teleport.CurrentTime() >= NUMBER_OF_TELEPORT_PIC * SECOND_PER_PICTURE_FASTER) {
                    if (chicken->OnTeleport()) {
                        chicken->teleport.resetTime();
                        chicken->getEntity()->setPosition(Rand(100, SCREEN_WIDTH - 100), 20);
                        SDL_Rect chicken_rect = chicken->getEntity()->getRect();
                        chicken->teleport.setRect(chicken_rect.x + chicken_rect.w/2 - chicken->teleport.getW()/2, chicken_rect.y + chicken_rect.h/2 - chicken->teleport.getH()/2);
                        chicken->setOnTeleport(true);
                        playChunk(Media::Instance()->tele);
                    }
                }
                else {
                    chicken->teleport.render(renderer);
                }
            }
            else {
                chicken->render(renderer);
            }

        }
        chicken->handleBullet(renderer, chickenBullets);
        chicken->handleExplosion(renderer);
    }

    if (topChicken != nullptr && topChicken->chicken_type() == CHICKEN_SMALL) {
        ChickenMoveState chickenMoveState = topChicken->getMoveState();

        if (touchBottom(bottomChicken, 5)) {
            chickenMoveState.goDown = 0;
            chickenMoveState.goUp = 1;
        }
        if (touchTop(topChicken)) {
            chickenMoveState.goDown = 1;
            chickenMoveState.goUp = 0;
        }
        if (touchRight(rightChicken)) {
            chickenMoveState.goRight = 0;
            chickenMoveState.goLeft = 1;
        }
        if (touchLeft(leftChicken)) {
            chickenMoveState.goRight = 1;
            chickenMoveState.goLeft = 0;
        }

        for (Chicken *chicken: chickens) chicken->setMoveState(chickenMoveState);
    }

    if (topChicken != nullptr && (topChicken->chicken_type() == CHICKEN_BOSS || topChicken->chicken_type() == CHICKEN_DODGE)) {
        ChickenType chicken_type = topChicken->chicken_type();
        if (bossTurnTimer.timeIsUp()) {
            for (Chicken *chicken: chickens) {
                ChickenMoveState moveState = chicken->getMoveState();
                int H_Rate = 50, V_Rate = 50;
//                int gundam_ind = Rand(0, num_players - 1);
                if (gundam[gundam_ind].getEntity()->getX() < chicken->getEntity()->getX()) {
                    H_Rate = chicken_type == CHICKEN_BOSS ? 80 : 30;
                }
                else {
                    H_Rate = chicken_type == CHICKEN_BOSS ? 30 : 80;
                }

                if (Rand(0, 100) < H_Rate) {
                    moveState.goLeft = 1;
                    moveState.goRight = 0;
                }
                else {
                    moveState.goRight = 1;
                    moveState.goLeft = 0;
                }
                if (Rand(0, 100) < V_Rate) {
                    moveState.goUp = 1;
                    moveState.goDown = 0;
                }
                else {
                    moveState.goDown = 1;
                    moveState.goUp = 0;
                }

                chicken->setMoveState(moveState);
            }
            bossTurnTimer.startCountdown();
        }

        for (Chicken *chicken: chickens) {
            ChickenMoveState moveState = chicken->getMoveState();
            double mul = chicken_type == CHICKEN_BOSS ? 0.5 : 1.5;
            if (touchBottom(chicken, mul)) {
                moveState.goDown = 0;
                moveState.goUp = 1;
            }
            if (touchTop(chicken)) {
                moveState.goDown = 1;
                moveState.goUp = 0;
            }
            if (touchRight(chicken)) {
                moveState.goRight = 0;
                moveState.goLeft = 1;
            }
            if (touchLeft(chicken)) {
                moveState.goRight = 1;
                moveState.goLeft = 0;
            }

            chicken->setMoveState(moveState);
        }
    }

    //....................fried chicken..................................

    if (!fried_chickens.empty()) {
        auto it = fried_chickens.begin();
        for (FriedChicken *fried_chicken: fried_chickens) {
            auto next_it = it;
            next_it ++;
            if (!fried_chicken->handleMove()) {
                fried_chickens.erase(it);
                removeFriedChicken(fried_chicken);
            }
            else {
                fried_chicken->render_friedchicken(renderer);
            }
            it = next_it;
        }
    }
}

void Game::init_rock() {
    if ((round == ROCK_FALL_ROUND || round == ROCK_SIDE_ROUND) && rockWaveCount > 0 && rockWaveTimer.timeIsUp()) {
        int n = 8 + game_difficulty * 2;
        int H_OFFSET = 700;
        int L = (round == ROCK_FALL_ROUND ? (SCREEN_WIDTH / 5) * 4 : (SCREEN_HEIGHT + H_OFFSET)) / n;
        for (int i = 0; i < n; ++ i) if (Rand(0, 10) < 6) {
            int x, y, step_x, step_y;
            if (round == ROCK_FALL_ROUND) {
                x = Rand(L * i - 100, L * (i + 1) + 100) ;
                y = -50;
                step_x = 0;
                step_y = Rand(MIN_ROCK_FALL_SPEED, MAX_ROCK_FALL_SPEED) + game_difficulty * NG_ROCK_SPEED;
            }
            else {
                x = -50;
                y = Rand(L * i - H_OFFSET, L * (i + 1) - H_OFFSET);
                step_x = Rand(MIN_ROCK_SIDE_SPEED_X, MAX_ROCK_SIDE_SPEED_X) + game_difficulty * NG_ROCK_SPEED;
                step_y = Rand(MIN_ROCK_SIDE_SPEED_Y, MAX_ROCK_SIDE_SPEED_Y) + game_difficulty * NG_ROCK_SPEED;
            }
            int _size = Rand(MIN_ROCK_SIZE, MAX_ROCK_SIZE);
            Rock *rock = new Rock(ROCK, {x, y, _size, _size}, Gallery::Instance()->rocks[Rand(0, _size(Gallery::Instance()->rocks) - 1)]);
            rock->setStep(step_x, step_y);
            rock->setHP(ROCK_HP + ROCK_HP_UPGRADE * game_difficulty);
            rock->setActive(true);
            rocks.insert(rock);
//                cout << "New Rock" << rock->getX() << " " << rock->getY() << " " << rock->getW() << " " << rock->getH() << "\n";
        }
        rockWaveCount --;
        rockWaveTimer.startCountdown();
    }
}

void Game::process() {
    if (status != GAME_PLAYING) return;

    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            setGameStatus(GAME_STOP);
            return;
        }
        if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
            for (int i = 0; i < num_players; ++ i) {
                gundam[i].control(*event);
            }
        }
        for (int i = 0; i < num_players; ++ i) {
            if (event->type == SDL_KEYDOWN && (event->key).keysym.sym == RocketKeyCode[i]) {
                if (!rocket.Active() && rocketCount > 0 && gundam[i].isAlive()) {
                    rocket.Set();
                    rocketCount[i] --;
                    playChunk(Media::Instance()->rocket);
                }
            }
        }
        if ((event->type == SDL_KEYDOWN && (event->key).keysym.sym == SDLK_ESCAPE)
            || (event->type == SDL_MOUSEBUTTONDOWN && isHover(*event, pause_button))
        )
        {
            playChunk(Media::Instance()->upgrade);
            setGameStatus(GAME_PAUSED);
            for (int i = 0; i < num_players; ++ i) {
                gundam[i].resetControl();
            }

            return;
        }
    }

    // ........................Timer process..............................
    initTimer.process();
    gameEndTimer.process();
    rockWaveTimer.process();
    bossTurnTimer.process();

    for (int i = 0; i < num_players; ++ i) {
        gundam[i].processTimer();
    }

    for (Chicken *chicken: chickens) if (chicken->isAlive()) {
        chicken->timerProcess();
    }

    if (round == MINI_BOSS_ROUND && !chickens.empty()) {
        bool round_end = true;
        for (Chicken *chicken: chickens) if (chicken->isAlive()) {
            round_end = false;
            break;
        }
        if (!round_end) roundTimer.process();
    }

    //...........................................................

    SDL_RenderClear(renderer);

    (scrolling += SCREEN_SPEED * TimeManager::Instance()->getElapsedTime());
    if (scrolling >= BG_SIZE) scrolling -= BG_SIZE;
    background.render(renderer, scrolling);

    // ............................gundam.......................................
    process_gundam();
    if (rocket.Active()) {
        rocket.handleMove();
        rocket.render(renderer);
    }
    rocket.handleExplosion(renderer);

    //...............................rock init..............................................
    init_rock();

    //..............................round...........................................
    if (isRoundWon()) {
//        cout << initTimer.timeIsUp() << "\n";
        if (initTimer.timeIsUp()) {
            if (round == BOSS_ROUND) {
//                setGameStatus(GAME_WON);
//                return;
                NG ++;
            }
//            cerr << "Start init\n";
            round = GameRound((round % ROUND_COUNT) + 1);
            init();
        }
        else {
            int nextRound = NG * ROUND_COUNT + round + 1;
            roundTitle.setText("Wave " + to_string(nextRound) + ": ");
            roundTitle.renderText(fontRoundTitle, renderer);
            roundText.setText(ROUND_TEXT[(round % ROUND_COUNT) + 1]);
            roundText.renderText(fontRoundText, renderer);

            int W = roundTitle.getW() + roundText.getW();
            roundTitle.setRect(SCREEN_WIDTH/2 - W/2, SCREEN_HEIGHT/2 - roundTitle.getH()/2 - 100);
            roundText.setRect(roundTitle.getX() + roundTitle.getW(), SCREEN_HEIGHT/2 - roundText.getH()/2 - 100);
            roundTitle.renderText(fontRoundTitle, renderer);
            roundText.renderText(fontRoundText, renderer);
        }
    }

    //......................................rock....................................................................................
    //................................chicken.........................................
    process_enemy();

    //................................upgrade................................................................
    for (auto *upgrade: upgrades) {
        upgrade->_move();
        upgrade->render(renderer);
        if (!upgrade->isInsideScreen()) {
            upgrades.erase(upgrade);
            delete(upgrade);
        }
    }

    //.............................explosion..................................................................
    while (!explosions.empty() && explosions.front()->CurrentTime() >= SECOND_PER_PICTURE * NUMBER_OF_EXPLOSION_PIC) {
        Entity *explosion = explosions.front();
        explosions.pop_front();
        delete(explosion);
    }
    for (auto *explosion: explosions) explosion->render(renderer);

    handleGameEvent();

    //.............................game over.......................................
    if (isGameOver()) {
        Text gameover("Game Over!", TEXT_COLOR);
        gameover.renderText(fontMenu, renderer, true);
        gameover.setRect(SCREEN_WIDTH/2 - gameover.getW()/2, SCREEN_HEIGHT/3);
        gameover.renderText(fontMenu, renderer);
        if (gameEndTimer.timeIsUp()) {
            setGameStatus(GAME_OVER);
        }
    }

    //health bar
    process_game_state();
    //...................pause........................................
    pause_button.render(renderer);
    //....................................................................
    if (round == MINI_BOSS_ROUND) {
//        cout << roundTimer.RemainTime() << "\n";
        string remain_time = "Time left: " + to_string(roundTimer.RemainTime());
        for (int i = 0; i < 3; ++ i) remain_time.pop_back();
//        remain_time += 's';
//        remain_time += " second";
        roundTimerText.setText(remain_time);
        roundTimerText.renderText(fontRoundText, renderer);
    }

    //gundam bullet
    for (int i = 0; i < num_players; ++ i) {
        gundam[i].handleBullet(renderer, enemy_positions);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}

void Game::dropUpgrade(EntityType eType) {
    if (eType != LEVEL_UP && eType != NEW_WEAPON) return;

    int x = Rand(50, SCREEN_WIDTH - 50), y = -10;
    UpgradeType uType = (eType == LEVEL_UP ? UPGRADE_LEVEL_UP : UpgradeType(Rand(0, NUMBER_OF_UPGRADE - 2)));
    Upgrade *upgrade = new Upgrade(uType, {x, y, 0, 0});
    upgrade->getEntity()->setStep(0, UPGRADE_SPEED);
    if (uType == UPGRADE_LEVEL_UP) {
        upgrade->getEntity()->setTexture(Gallery::Instance()->levelUp);
    }
    else {
        upgrade->getEntity()->setTexture(Gallery::Instance()->newWeapons[uType]);
    }
    upgrades.insert(upgrade);
}

void Game::setRoundWon() {
    cout << "Round Won\n";
    roundWon = true;
    initTimer.startCountdown();
    score += ROUND_SCORE[round - 1] + NG_ROUND_SCORE[round - 1] * game_difficulty;
}

void Game::handleGameEvent() {
    //.............................upgrade.....................................................
    for (Upgrade *upgrade: upgrades) for (int i = 0; i < num_players; ++ i) {
        if (gundam[i].isAlive() && upgrade->getEntity()->collisionWith(gundam[i].getEntity())) {
//            cout << "Level Up\n";
            if (upgrade->getType() == UPGRADE_LEVEL_UP) {
                gundam[i].levelUp();
            }
            else {
//                cout << upgrade->getType() << "\n";
                UpgradeType uType = upgrade->getType();
                if (uType == UPGRADE_ADD_LASER) {
//                    cout << "GET LASER\n";
                    gundam[i].laserTimer.startCountdown();
                }
                else {
                    gundam[i].addWeapon(WeaponType(uType));
                }
            }
//            upgrades.erase(upgrade);
            removeUpgrade(upgrade);
            playChunk(Media::Instance()->upgrade);
        }
    }

    //............................chicken...................................................
    for (Chicken *chicken: chickens) for (int i = 0; i < num_players; ++ i) {
        if (chicken->isAlive()) {
            if (!chicken->OnRocket() && chicken->getEntity()->collisionWith(gundam[i].getEntity())) {
                if (gundam[i].isAlive()) {
                    gundamDead(i);
                }
            }
        }

        if (gundam[i].isAlive() && chicken->checkBulletCollision(gundam[i].getEntity())) {
            gundamDead(i);
        }

        if (chicken->isAlive() && !chicken->OnRocket()) {
            set<Bullet*> gundamBullets = gundam[i].getBullets();
            for (Bullet *bullet: gundamBullets) {
                if (chicken->getEntity()->collisionWith((bullet->getEntity()))) {
//                    cout << "abc\n";
                    gundam[i].removeBullet(bullet);
                    chickenReceiveDamage(chicken, gundam[i].getBulletDamage());
                    if (!chicken->isAlive()) break;
                }
            }

            if (chicken->isAlive() && gundam[i].isLaserOn() && chicken->getEntity()->collisionWith(gundam[i].getLaser())) {
                const double gundamLaserDamage = (GUNDAM_LASER_DAMAGE + GUNDAM_LASER_UPGRADE * gundam[i].getLevel()) * TimeManager::Instance()->getElapsedTime();
                chickenReceiveDamage(chicken, gundamLaserDamage);
            }
        }
    }

    //...............................rock..........................................
    for (Rock *rock: rocks) if (rock->isActive()) for (int i = 0; i < num_players; ++ i) {
        if (gundam[i].isAlive() && rock->collisionWith(gundam[i].getEntity())) {
            gundamDead(i);
        }

        set<Bullet*> gundamBullets = gundam[i].getBullets();

        for (Bullet *bullet: gundamBullets) {
            if (rock->collisionWith((bullet->getEntity()))) {
                gundam[i].removeBullet(bullet);
                rockReceiveDamage(rock, gundam[i].getBulletDamage());
            }
        }

        if (rock->isActive() && gundam[i].isLaserOn() && rock->collisionWith(gundam[i].getLaser())) {
            const double gundamLaserDamage = (GUNDAM_LASER_DAMAGE + GUNDAM_LASER_UPGRADE * gundam[i].getLevel()) * TimeManager::Instance()->getElapsedTime();
            rockReceiveDamage(rock, gundamLaserDamage);
        }

    }

    if ((round == ROCK_FALL_ROUND || round == ROCK_SIDE_ROUND) && rockWaveCount == 0 && rocks.empty() && !roundWon) {
        setRoundWon();
    }

    //...............................fried chicken................................
    for (int i = 0; i < num_players; ++ i) {
        if (!fried_chickens.empty()) {
            auto it = fried_chickens.begin();
            for (FriedChicken *fried_chicken: fried_chickens) {
                auto next_it = it;
                next_it ++;
                if (fried_chicken->collisionWith(gundam[i].getEntity())) {
                    fried_chickens.erase(it);
                    removeFriedChicken(fried_chicken);
                    frychickenCount[i] ++;
                    if (frychickenCount[i] >= FRIED_CHICKEN_TO_ROCKET) {
                        frychickenCount[i] -= FRIED_CHICKEN_TO_ROCKET;
                        rocketCount[i] ++;
                    }
                    playChunk(Media::Instance()->bite);
                }
                it = next_it;
            }
        }
    }

    //...............rocket......................................
    if (rocket.reached(audioState == AUDIO_UNMUTED)) {
        for (Chicken *chicken: chickens) {
            if (chicken->isAlive()) chickenReceiveDamage(chicken, rocket_damage);
            set<Bullet*> bullets = chicken->getBullets();
            for (Bullet *bullet: bullets) chicken->removeBullet(bullet, chickenBullets);
        }
        for (Rock *rock: rocks) {
            if (rock->isActive()) rockReceiveDamage(rock, rocket_damage);
        }
    }

    //......................speed round (mini boss) ....................................
    if (!roundWon && round == MINI_BOSS_ROUND && roundTimer.timeIsUp()) {
        for (int i = 0; i < num_players; ++ i) gundamDead(i, true);
    }
}

void Game::addExplosion(SDL_Rect rect, int level) {
    Texture texture = Gallery::Instance()->expolosions[level];
//    int w = texture.w / NUMBER_OF_EXPLOSION_PIC, h = texture.h;
//    int center_x = (rect.x + rect.w/2), center_y = (rect.y + rect.h/2);
//    cout << center_x - w/2 << " " << center_y - h/2 << "\n";
//    explosions.push_back(new Entity(EXPLOSION, {center_x - w/2, center_y - h/2, w, h}, texture));
    explosions.push_back(new Entity(EXPLOSION, rect, texture));
}

void Game::gundamDead(int ind, bool immediately) {
    if (gundam[ind].getLives() == 0) return;

    if (immediately) {
        while (gundam[ind].getLives() > 0) {
//            hearts[ind].pop_back();
            gundam[ind].dead();
        }
    }

    if (!gundam[ind].shieldTimer.timeIsUp()) {
        playChunk(Media::Instance()->bulletRock);
        return;
    }
    gundam[ind].dead();
//    hearts[ind].pop_back();

    addExplosion(gundam[ind].getEntity()->getRect(), 1);
    playChunk(Media::Instance()->explosions[0]);
    gundam[ind].reviveTimer.startCountdown();

    for (int i = 0; i < num_players; ++ i) {
        if (gundam[ind].getLives() == 0) {
            gameEndTimer.startCountdown();
        }
    }
}

void Game::addFriedChicken(double x, double y, int level) {
    int cnt = NUMBER_OF_FRIED_CHICKEN[level];
    while (cnt --) {
        FriedChicken *fried_chicken = new FriedChicken(x, y);
        fried_chicken->setTextures(Gallery::Instance()->fried_chickens);
        fried_chickens.push_back(fried_chicken);
    }
}

void Game::chickenDead(Chicken *chicken) {
//                    chickens.erase(chicken);
    if (chicken->chicken_type() == CHICKEN_SMALL) {
        if (Rand(0, 100) < 50) playChunk(Media::Instance()->chickens[2]);
    }
    else {
        playChunk(Media::Instance()->explosions[1]);
    }

    addExplosion(chicken->getEntity()->getRect(), chicken->chicken_type() == CHICKEN_BOSS);
    addFriedChicken(chicken->getEntity()->get_act_x(), chicken->getEntity()->get_act_y(), chicken->chicken_type());

    int chickenLevel = chicken->chicken_type() == CHICKEN_SMALL ? 0 : 1;
    int killed = (++ killedChickenCount[chickenLevel]);
    if (chickenLevel == 0 && killed % CHICKENS_TO_LEVEL_UP == 0) {
        if (Rand(0, 100) < 60) dropUpgrade(LEVEL_UP);
        else dropUpgrade(NEW_WEAPON);
    }
    if (chickenLevel == 1 && killed % BOSS_TO_NEW_WEAPON == 0) {
        if (Rand(0, 100) < 40) dropUpgrade(LEVEL_UP);
        else dropUpgrade(NEW_WEAPON);
    }

    numberOfAliveChicken --;
    if (numberOfAliveChicken == 0) {
        setRoundWon();
//                        cerr << "Round won\n";
    }

    score += CHICKEN_SCORE[chicken->chicken_type()] + NG_CHICKEN_SCORE[chicken->chicken_type()] * NG;
}

void Game::chickenReceiveDamage(Chicken *chicken, double damage) {
    if (chicken->chicken_type() == CHICKEN_DODGE && chicken->teleportCooldown.timeIsUp()) {
        chicken->teleportCooldown.startCountdown();
        chicken->teleportDuration.startCountdown();
        chicken->teleport.resetTime();
        SDL_Rect chicken_rect = chicken->getEntity()->getRect();
        chicken->teleport.setRect(chicken_rect.x + chicken_rect.w/2 - chicken->teleport.getW()/2, chicken_rect.y + chicken_rect.h/2 - chicken->teleport.getH()/2);
        chicken->setOnTeleport(true);
        playChunk(Media::Instance()->tele);
    }
    else {
        bool alive = chicken->receiveDamage(damage);
        if (!alive) {
            chickenDead(chicken);
        }
        else {
            if (Rand(0, 100) < 30) playChunk(Media::Instance()->chickens[Rand(0, 1)]);
        }
    }
}

void Game::rockReceiveDamage(Rock *rock, double damage) {
    bool alive = true;
    if (!rock->receiveDamage(damage)) alive = false;
    playChunk(Media::Instance()->bulletRock);
    if (!alive) {
        addExplosion(rock->getRect(), 1);
    }
}
//.......................................menu...............................
void Game::toggleAudio() {
    audioState = GameAudio((audioState + 1) % AUDIO_COUNT);
    if (audioState == AUDIO_MUTED) {
        Mix_PauseMusic();
    }
    else {
        Mix_ResumeMusic();
    }
}

void Game::renderMenu() {
    if (status != GAME_INITALIZING) return;

    SDL_RenderClear(renderer);

    for (int i = 1; i < _size(settingsMenuChoice); ++ i) {
        if (i == SETTING_MENU_AUDIO) {
            settingsMenuChoice[i].setText(GAME_AUDIO[audioState]);
        }
        if (i == SETTING_MENU_DIFFICULTY) {
            settingsMenuChoice[i].setText(GAME_DIFFICULTY[difficultyState]);
        }
        settingsMenuChoice[i].renderText(fontMenu, renderer);
        settingsMenuChoice[i].setRect(SCREEN_WIDTH*3/4 - settingsMenuChoice[i].getW()/2, 150 + (i - 1) * 80);
    }

    vector<Text> texts = {};
    vector<Text> &choices = (menuState == MENU_MAIN ? mainMenuChoice : (
                             menuState == MENU_PLAY ? playMenuChoice : (
                             menuState == MENU_SETTINGS ? settingsMenuChoice : (
                             menuState == MENU_RANKING ? rankingMenuChoice : (
                             controlMenuChoice)))));
    if (menuState == MENU_MAIN) {
        choices = mainMenuChoice;
        menu.render(renderer);
    }
    else if(menuState == MENU_PLAY) {
        choices = playMenuChoice;
        menu_play.render(renderer);
    }
    else if (menuState == MENU_SETTINGS) {
        texts = settingsMenuText;
        choices = settingsMenuChoice;
        menu_settings.render(renderer);
    }
    else if (menuState == MENU_RANKING) {
        choices = rankingMenuChoice;
        menu_settings.render(renderer);
        showRanking();
    }
    else {
        choices = controlMenuChoice;
        menu_control.render(renderer);
    }

//    cout << _size(texts) << "\n";
    for (int i = 0; i < _size(texts); ++ i) {
        texts[i].renderText(fontMenu, renderer);
    }
    for (int i = 0; i < _size(choices); ++ i) {
        SDL_Rect rect = choices[i].getRect();
        choices[i].renderText(choices[i].color_equal(TEXT_HOVER_COLOR) ? fontMenuHover : fontMenu, renderer, true);
        choices[i].setRect(rect.x + rect.w/2 - choices[i].getW()/2, rect.y + rect.h/2 - choices[i].getH()/2);
        choices[i].renderText(choices[i].color_equal(TEXT_HOVER_COLOR) ? fontMenuHover : fontMenu, renderer);
    }
    if (menuState == MENU_SETTINGS) {
        BGM.render(renderer);
        SFX.render(renderer);
    }

    while (SDL_PollEvent(event) != 0) {
        BGM.handleEvent(*event);
        SFX.handleEvent(*event);
        Mix_VolumeMusic(MIX_MAX_VOLUME * BGM.Value());
        Mix_Volume(-1, MIX_MAX_VOLUME * SFX.Value());
        if (event->type == SDL_QUIT) {
            setGameStatus(GAME_STOP);
        }
        else if (event->type == SDL_KEYDOWN) {
            if ((event->key).keysym.sym == SDLK_ESCAPE && menuState != MENU_MAIN) {
                menuState = MENU_MAIN;
            }
        }
        else if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN) {
            for (int i = 0; i < _size(choices); ++ i) {
                if (isHover(*event, choices[i])) {
                    if (event->type == SDL_MOUSEMOTION) {
                        choices[i].setColor(TEXT_HOVER_COLOR);
//                            cout << "hover\n";
                    }
                    else {
                        playChunk(Media::Instance()->upgrade);
                        if (menuState == MENU_MAIN) {
                            if (i == MAIN_MENU_START) {
                                menuState = MENU_PLAY;
//                                setGameStatus(GAME_PLAYING);
//                                initTimer.startCountdown();
                            }
                            if (i == MAIN_MENU_SETTINGS) {
                                menuState = MENU_SETTINGS;
                            }
                            if (i == MAIN_MENU_RANKING) {
                                menuState = MENU_RANKING;
                            }
                            if (i == MAIN_MENU_CONTROL) {
                                menuState = MENU_CONTROL;
                            }
                            if (i == MAIN_MENU_QUIT) {
                                setGameStatus(GAME_STOP);
                            }
                        }
                        else {
                            if (i == 0) {
                                menuState = MENU_MAIN;
                            }
                            if (menuState == MENU_PLAY) {
                                if (i == PLAY_MENU_SINGLE_PLAYER) {
                                    mode = GAME_SINGLE_PLAYER;
                                    num_players = 1;
                                }
                                if (i == PLAY_MENU_MULTIPLAYER) {
                                    mode = GAME_MULTIPLAYER;
                                    num_players = 2;
                                }
                                setGameStatus(GAME_PLAYING);
                                initTimer.startCountdown();
                                menuState = MENU_MAIN;
                                initData();
                            }
                            if (menuState == MENU_SETTINGS) {
                                if (i == SETTING_MENU_AUDIO) {
                                    toggleAudio();
                                }
                                else if (i == SETTING_MENU_DIFFICULTY) {
                                    difficultyState = GameDifficulty((difficultyState + 1) % GAME_DIFFICULTY_COUNT);
                                }
                            }
                            if (menuState == MENU_RANKING) {
                                if (i == RANKING_MENU_CLEAR_RANKING) {
                                    scores.clear();
                                }
                            }
                        }
                    }
                }
                else {
                    choices[i].setColor(TEXT_COLOR);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}

void Game::renderPauseMenu() {
    if (status != GAME_PAUSED) return;

    SDL_RenderClear(renderer);

    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            setGameStatus(GAME_STOP);
            return;
        }
        if (event->type == SDL_MOUSEBUTTONDOWN) {
            if (isHover(*event, home_button)) {
                playChunk(Media::Instance()->upgrade);
                playAgain();
                return;
            }
            if (isHover(*event, resume_button)) {
                playChunk(Media::Instance()->upgrade);
                setGameStatus(GAME_PLAYING);
                return;
            }
            if (isHover(*event, audio_button)) {
                playChunk(Media::Instance()->upgrade);
                toggleAudio();
            }
        }
        if (event->type == SDL_KEYDOWN && (event->key).keysym.sym == SDLK_ESCAPE) {
            playChunk(Media::Instance()->upgrade);
            setGameStatus(GAME_PLAYING);
            return;
        }
    }

//    cout << "paused\n";

    pause_menu.render(renderer);
    home_button.render(renderer);
    resume_button.render(renderer);
    audio_button.setTexture((audioState == AUDIO_MUTED ? Gallery::Instance()->audio_muted_button : Gallery::Instance()->audio_unmuted_button));
    audio_button.render(renderer);

    SDL_RenderPresent(renderer);
}

//..............................data..........................................
void Game::initData() {
    _clear();
    score = NG = 0;
    round = INIT_ROUND;
//    round = ROCK_FALL_ROUND;
//    audioState = AUDIO_MUTED;
    scrolling = 0;

    for (int i = 0; i < num_players; ++ i) {
        rocketCount[i] = 0;
        if (difficultyState == GAME_EASY) rocketCount[i] = 3;
        if (difficultyState == GAME_NORMAL) rocketCount[i] = 1;
        frychickenCount[i] = 0;
    }
    roundWon = true;

    killedChickenCount.assign(5, 0);
    chickens.clear(); chickenBullets.clear();
    rocks.clear();
    upgrades.clear();
    explosions.clear();
//    hearts.clear();

//    hearts.clear();
//    hearts.resize(2);
//    for (int ind = 0; ind < num_players; ++ ind) {
//        for (int i = 0; i < gundam[ind].getLives(); ++ i) {
//            hearts[ind].push_back(Entity(HEART, {10 + i * 27, 10, 25, 25}));
//            hearts[ind].back().setTexture(Gallery::Instance()->heart);
//        }
//    }
}

vector<pair<int, string>> Game::getRanking() {
    vector<pair<int, string>> v;
    for (auto tmp: scores) v.push_back(make_pair(tmp.second, tmp.first));
    sort(v.begin(), v.end(), greater<pair<int, string>>());
    while (_size(v) > NUMBER_SHOWED_PLAYER) v.pop_back();
    return v;
}

void Game::saveData() {
    ofstream fout("./assets/data/players.txt");

    auto v = getRanking();
    fout << _size(v) << "\n";
    for (auto tmp: v) fout << tmp.second << " " << tmp.first << "\n";

    fout.close();
}

void Game::load() {
    setGameStatus(GAME_INITALIZING);
//    playMusic(Media::Instance()->start);
    TTF_Init();
    fontMenu = TTF_OpenFont("./assets/font/Zebulon Bold.otf", 36);
    fontMenuHover = TTF_OpenFont("./assets/font/Zebulon Bold.otf", 40);
    fontGame = TTF_OpenFont("./assets/font/Zebulon.otf", 24);
    fontRoundTitle = TTF_OpenFont("./assets/font/Zebulon Bold.otf", 32);
    fontRoundText = TTF_OpenFont("./assets/font/Zebulon.otf", 20);
    fontGundamState = TTF_OpenFont("./assets/font/Zebulon.otf", 16);

    initData();

    const int offset = 10;

    scoreText.setText("Score: ");
    scoreText.setColor(TEXT_COLOR);
    scoreText.renderText(fontGame, renderer, true);
    scoreText.setRect(offset, offset + offset/2);
    scoreValue.setText("0");
    scoreValue.setColor(TEXT_COLOR);
    scoreValue.renderText(fontRoundText, renderer, true);
//    cout << scoreText.getW() << "\n";
    scoreValue.setRect(offset + scoreText.getW(), offset + offset/2 + scoreText.getH() - scoreValue.getH());

    const int gundamStateOffsetY = scoreText.getY() + scoreText.getH() + offset/2;
    for (int i = 0; i < 2; ++ i) {
        int offset_x = i == 0 ? 0 : 1050;
        gundamLevelImage[i].setTexture(Gallery::Instance()->level);
        gundamLevelImage[i].setRect({0, 0, 8, 20});
//        gundamLevelImage[i].setRect(offset + offset_x, gundamStateOffsetY);
        gundamLevelText[i].setText(to_string(gundam[i].getLevel()));
        gundamLevelText[i].setColor(TEXT_COLOR);
//        gundamLevelText[i].setRect(gundamLevelImage[i].getX() + gundamLevelImage[i].getW() + offset/2, gundamStateOffsetY);

        frychickenMini[i].setTexture(Gallery::Instance()->fry_chicken_mini);
        frychickenMini[i].setRect({0, 0, 26, 20});
//        frychickenMini[i].setRect(gundamLevelText[i].getX() + gundamLevelText[i].getW() + offset + offset_x, gundamStateOffsetY);
        frychickenText[i].setText(to_string(frychickenCount[i]));
        frychickenText[i].setColor(TEXT_COLOR);
//        frychickenText[i].setRect(frychickenMini[i].getX() + frychickenMini[i].getW() + offset/2, gundamStateOffsetY);

        rocketMini[i].setTexture(Gallery::Instance()->rocket_mini);
        rocketMini[i].setRect({0, 0, 6, 20});
//        rocketMini[i].setRect(frychickenText[i].getX() + frychickenText[i].getW() + offset + offset_x, gundamStateOffsetY);
        rocketText[i].setText(to_string(rocketCount[i]));
        rocketText[i].setColor(TEXT_COLOR);
//        rocketText[i].setRect(rocketMini[i].getX() + rocketMini[i].getW() + offset/2, gundamStateOffsetY);

        heart[i].setTexture(Gallery::Instance()->heart);
        heart[i].setRect({0, 0, 20, 20});
        heartText[i].setText("5");
        heartText[i].setColor(TEXT_COLOR);
    }

    //....................menu.....................................
    mainMenuChoice = {
        Text("Save the World", TEXT_COLOR),
        Text("Settings", TEXT_COLOR),
        Text("Control", TEXT_COLOR),
        Text("Hall of Fame", TEXT_COLOR),
        Text("Quit", TEXT_COLOR),
    };
    settingsMenuText = {
        Text("", TEXT_COLOR),
        Text("Difficulty", TEXT_COLOR),
        Text("Audio", TEXT_COLOR),
        Text("BGM", TEXT_COLOR),
        Text("SFX", TEXT_COLOR),
    };
    backButton.renderText(fontRoundTitle, renderer, true);
    backButton.setRect(SCREEN_WIDTH - backButton.getW() - 30, SCREEN_HEIGHT - backButton.getH() - 20);
    playMenuChoice = {
        backButton,
        Text("Single player", TEXT_COLOR),
        Text("Multiplayer", TEXT_COLOR)
    };
    settingsMenuChoice = {
        backButton,
        Text("", TEXT_COLOR),
        Text("", TEXT_COLOR)
    };
    rankingMenuChoice = {
        backButton,
        Text("Reset ranking", TEXT_COLOR)
    };
    rankingMenuChoice[1].renderText(fontMenu, renderer, true);
    rankingMenuChoice[1].setRect(30, SCREEN_HEIGHT - rankingMenuChoice[1].getH() - 20);

    controlMenuChoice = {backButton};

    for (int i = 0; i < _size(mainMenuChoice); ++ i) {
        mainMenuChoice[i].renderText(fontMenu, renderer, true);
        mainMenuChoice[i].setRect(SCREEN_WIDTH/2 - mainMenuChoice[i].getW()/2, SCREEN_HEIGHT - 320 + i * 60);
    }
    for (int i = 1; i < _size(playMenuChoice); ++ i) {
        playMenuChoice[i].renderText(fontMenu, renderer, true);
        playMenuChoice[i].setRect(SCREEN_WIDTH/2 - playMenuChoice[i].getW()/2, SCREEN_HEIGHT - 380 + i * 60);
    }
    for (int i = 1; i < _size(settingsMenuText); ++ i) {
        settingsMenuText[i].renderText(fontMenu, renderer, true);
        settingsMenuText[i].setRect(SCREEN_WIDTH/4 + 50 * (i > 2), 150 + (i - 1) * 80);
    }
    BGM.init(SCREEN_WIDTH*3/4, settingsMenuText[3].getY() + settingsMenuText[3].getH()/2);
    SFX.init(SCREEN_WIDTH*3/4, settingsMenuText[4].getY() + settingsMenuText[4].getH()/2);


    //..................................................................
    roundTimerText.setText("Time left: 0.000");
    roundTimerText.setColor(TEXT_COLOR);
    roundTimerText.renderText(fontRoundText, renderer, true);
    roundTimerText.setRect(SCREEN_WIDTH/2 - roundTimerText.getW()/2, 0);

    ifstream fin("./assets/data/players.txt");

    string playerName;
    int playScore;
    int numPlayers = 0;
    fin >> numPlayers;
    for (int i = 0; i < numPlayers; ++ i) {
        fin >> playerName >> playScore;
        scores[playerName] = playScore;
    }

    fin.close();
}

void Game::quit() {
    saveData();
    TTF_Quit();
}

void Game::playChunk(Mix_Chunk *chunk, int channel, int loop) {
    if (audioState == AUDIO_MUTED) return;
//    cout << channel << " " << loop << "\n";
    Mix_PlayChannel(channel, chunk, loop);
}

void Game::playMusic(Mix_Music *music) {
    Mix_PlayMusic(music, -1);
    if (audioState == AUDIO_MUTED) Mix_PauseMusic();
}

//...................................summary............................
void Game::gameOver() {
    if (status != GAME_OVER) return;
//    playMusic(Media::Instance()->ending);

    enterYourName();
    if (!isRunning()) return;

    Text nextButton("Next", TEXT_COLOR);
    nextButton.renderText(fontRoundTitle, renderer, true);
    nextButton.setRect(SCREEN_WIDTH - nextButton.getW() - 100, SCREEN_HEIGHT - nextButton.getH() - 50);

    SDL_Event e;
    bool rankShowing = true;
    while (rankShowing) {
        SDL_RenderClear(renderer);

        (scrolling += SCREEN_SPEED * TimeManager::Instance()->getElapsedTime());
        if (scrolling >= BG_SIZE) scrolling -= BG_SIZE;
        background.render(renderer, scrolling);
        showRanking();
        nextButton.renderText(fontRoundTitle, renderer);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                setGameStatus(GAME_STOP);
                rankShowing = false;
                return;
            }
            if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
                if (isHover(e, nextButton)) {
                    if (e.type == SDL_MOUSEMOTION) {
                        nextButton.setColor(TEXT_HOVER_COLOR);
                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        playChunk(Media::Instance()->upgrade);
                        rankShowing = false;
                        break;
                    }
                }
                else {
                    nextButton.setColor(TEXT_COLOR);
                }
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    playAgain();
}

void Game::enterYourName() {
    scoreText.setRect(SCREEN_WIDTH/2 - (scoreText.getW() + scoreValue.getW())/2, SCREEN_HEIGHT/3);
    scoreValue.setRect(scoreText.getX() + scoreText.getW(), SCREEN_HEIGHT/3 + scoreText.getH() - scoreValue.getH());

    Text placeholder("Enter your name!", TEXT_PLACEHOLDER_COLOR);
    placeholder.renderText(fontMenu, renderer, true);
    placeholder.setRect(SCREEN_WIDTH/2 - placeholder.getW()/2, SCREEN_HEIGHT/3 + 50);

    Text input("", TEXT_COLOR);
    input.setRect(SCREEN_WIDTH/2 - placeholder.getW()/2, SCREEN_HEIGHT/3);
    string username = "";

    Text nextButton("Next", TEXT_COLOR);
    nextButton.renderText(fontRoundTitle, renderer, true);
    nextButton.setRect(SCREEN_WIDTH/2 - nextButton.getW()/2, SCREEN_HEIGHT/3 + 150);

    bool summarizing = true;
    SDL_Event e;
    while (summarizing) {
        SDL_RenderClear(renderer);

        (scrolling += SCREEN_SPEED * TimeManager::Instance()->getElapsedTime());
        if (scrolling >= BG_SIZE) scrolling -= BG_SIZE;
        background.render(renderer, scrolling);

        scoreText.renderText(fontGame, renderer);
        scoreValue.renderText(fontRoundText, renderer);

        input.setText(username);
        input.renderText(fontMenu, renderer, true);
        input.setRect(SCREEN_WIDTH/2 - input.getW()/2, placeholder.getY());

        if (_size(username) == 0) {
            placeholder.renderText(fontMenu, renderer);
        }
        else {
            input.renderText(fontMenu, renderer);
        }
        nextButton.renderText(fontRoundTitle, renderer);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                setGameStatus(GAME_STOP);
                summarizing = false;
                return;
            }
            if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
                if (isHover(e, nextButton)) {
                    if (e.type == SDL_MOUSEMOTION) {
                        nextButton.setColor(TEXT_HOVER_COLOR);
                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN && _size(username) > 0) {
                        summarizing = false;
                        playChunk(Media::Instance()->upgrade);
                        if (!scores.count(username)) {
                            scores.insert({username, score});
                        }
                        else {
                            if (score > scores[username]) scores[username] = score;
                        }
                        break;
                    }
                }
                else {
                    nextButton.setColor(TEXT_COLOR);
                }
            }
            if (e.type == SDL_KEYDOWN) {
                playChunk(Media::Instance()->bulletRock);
                for (int i = 0; i < NUM_KEYCODES; ++ i) if (e.key.keysym.sym == KEYCODES[i] && _size(username) < MAX_NAME_LENGTH) {
                    char c = (i < 26) ? char('a' + i) : char('0' + i - 26);
                    username += c;
                    break;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE && _size(username) > 0) {
                    username.pop_back();
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }
}

void Game::showRanking() {
    vector<pair<int,string>> v = getRanking();
    for (int i = 0; i < NUMBER_SHOWED_PLAYER; ++ i) {
        playerNames[i].setColor(TEXT_COLOR);
        playerScores[i].setColor(TEXT_COLOR);
        if (i < _size(v)) {
            playerNames[i].setText(v[i].second);
            playerScores[i].setText(to_string(v[i].first));
        }
        else {
            playerNames[i].setText("Empty...");
            playerScores[i].setText("0");
        }
        playerNames[i].renderText(fontGame, renderer, true);
        playerNames[i].setRect(SCREEN_WIDTH/4, 100 + i * 50);
        playerScores[i].renderText(fontGame, renderer, true);
        playerScores[i].setRect(SCREEN_WIDTH*3/4 - playerScores[i].getW(), 100 + i * 50);

        playerNames[i].renderText(fontGame, renderer);
        playerScores[i].renderText(fontGame, renderer);
    }
}

void Game::playAgain() {
    reset();
    setGameStatus(GAME_INITALIZING);
}

void Game::reset() {
    setGameStatus(GAME_STOP);

    gundam[0].deactiveTimer();
    gundam[1].deactiveTimer();
    initTimer.deactive();
//    chickenTeleportCooldown.deactive();
//    chickenTeleportDuration.deactive();

    gundam[0].reset();
    gundam[1].reset();

    initData();

    scoreText.setRect(10, 10 + 5);
    scoreValue.setRect(10 + scoreText.getW(), 10 + 5 + scoreText.getH() - scoreValue.getH());
}

bool Game::isRoundWon() {
    for (int i = 0; i < num_players; ++ i) if (gundam[i].getLives() <= 0) return 0;
    return roundWon;
}

bool Game::isGameOver() {
    for (int i = 0; i < num_players; ++ i) if (gundam[i].getLives() > 0) return 0;
    return 1;
}
