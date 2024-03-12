#include "game.h"

Game::Game(SDL_Renderer *_renderer, SDL_Event *_event, Painter *_painter, int _width, int _height):
    renderer(_renderer), event(_event), painter(_painter),
    gallery(new Gallery(painter)),
    width(_width), height(_height),
    background(BACKGROUND, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}),
    initTimer(INIT_DELAY), gameEndTimer(GAME_END_DELAY),
    gundamReviveTimer(GUNDAM_REVIVE_TIME), gundamShieldTimer(GUNDAM_SHIELD_DURATION), gundamLaserTimer(GUNDAM_LASER_DURATION),
    roundTitle("", WHITE_COLOR), roundText("", WHITE_COLOR),
    gundam(gallery)
{
    media = new Media();

    setGameStatus(GAME_STOP);
    difficultyState = GAME_EASY;
    audioState = AUDIO_UNMUTED;
    score = 0;
    round = 0;
    roundWon = true;

    chickenMoveState = {0, 1, 0, 0};

    background.setTexture(gallery->background);
//    gundam.getEntity()->setTexture(gallery->gundams[gundam.getCurrentWeapon()]);

    killedChickenCount.assign(5, 0);

    gundam.setGame(this);
//    gundamLaserTimer.startCountdown();
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
        int numberOfBullet = (level ? perRow * 2: perRow) * (NG * 2 + 1);

        chickenMoveState = {0, 1, 0, !level};

        for (int i = 0; i < numberOfBullet; ++ i) chickenBullets.push_back(new Bullet());
        for (int i = 0; i < numberOfEnemy; ++ i) {
            int row = i / perRow, col = i % perRow;
            Chicken *chicken = new Chicken(col, row, level, NG);
//            chicken->getEntity()->setTexture(gallery->chickens[chicken->getLevel()]);
            if (round == BOSS_ROUND) {
                chicken->getEntity()->setTextures(gallery->chickens[chicken->getLevel() + i]);
            }
            else {
                chicken->getEntity()->setTextures(gallery->chickens[chicken->getLevel()]);
            }
            chickens.push_back(chicken);
        }
        numberOfAliveChicken = numberOfEnemy;
    }

//    gundamLaserTimer.startCountdown();
//    cout << "Init Successful\n";
}

void Game::process() {
    if (status != GAME_PLAYING) return;

    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            setGameStatus(GAME_STOP);
            return;
        }
        if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
            gundam.control(*event, gundamLaserTimer);
        }
    }

    painter->clearWithBgColor(WHITE_COLOR);


    (scrolling += SCREEN_SPEED) %= BG_SIZE;
    background.render(renderer, scrolling);

    //...................heart and score........................................
    for (auto &heart: hearts) heart.render(renderer);
    scoreText.renderText(fontGame, renderer);
    scoreValue.setText(to_string(score));
    scoreValue.renderText(fontRoundText, renderer);

    // ............................gundam.......................................
    if (gundamLaserTimer.timeIsUp()) gundam.setLaserOn(false);
    if (!gundam.isAlive() && gundamReviveTimer.timeIsUp()) {
        if (gundam.revive()) gundamShieldTimer.startCountdown();
    }
    gundam._move();
//    cout << (gallery->gundam).w << " " << (gallery->gundam).h << "\n";
    gundam.render(renderer, gallery, !gundamShieldTimer.timeIsUp(), !gundamLaserTimer.timeIsUp());
    gundam.handleBullet(renderer);


    //...............................rock init..............................................
    int rockWaveCount = (round == ROCK_FALL_ROUND ? ROCK_FALL_WAVE : ROCK_SIDE_WAVE) + NG * NG_ROCK_WAVE;
    if ((round == ROCK_FALL_ROUND || round == ROCK_SIDE_ROUND) && frame <= (rockWaveCount - 1) * ROCK_WAVE_FRAME) {
        if (frame % ROCK_WAVE_FRAME == 0) {
            int n = 10 + NG * 10;
            int H_OFFSET = 700;
            int L = (round == ROCK_FALL_ROUND ? (SCREEN_WIDTH / 5) * 4 : (SCREEN_HEIGHT + H_OFFSET)) / n;
            for (int i = 0; i < n; ++ i) if (Rand(0, 10) < 8) {
                int x, y, step_x, step_y;
                if (round == ROCK_FALL_ROUND) {
                    x = Rand(L * i - 100, L * (i + 1) + 100) ;
                    y = -50;
                    step_x = 0;
                    step_y = Rand(MIN_ROCK_FALL_SPEED, MAX_ROCK_FALL_SPEED) + NG * NG_ROCK_SPEED;
                }
                else {
                    x = -50;
                    y = Rand(L * i - H_OFFSET, L * (i + 1) - H_OFFSET);
                    step_x = Rand(MIN_ROCK_SIDE_SPEED_X, MAX_ROCK_SIDE_SPEED_X) + NG * NG_ROCK_SPEED;
                    step_y = Rand(MIN_ROCK_SIDE_SPEED_Y, MAX_ROCK_SIDE_SPEED_Y) + NG * NG_ROCK_SPEED;
                }
                int _size = Rand(MIN_ROCK_SIZE, MAX_ROCK_SIZE);
                Rock *rock = new Rock(ROCK, {x, y, _size, _size}, gallery->rocks[Rand(0, _size(gallery->rocks) - 1)]);
                rock->setStep(step_x, step_y);
                rock->setHP(ROCK_HP + ROCK_HP_UPGRADE * NG);
                rock->setActive(true);
                rocks.insert(rock);
//                cout << "New Rock" << rock->getX() << " " << rock->getY() << " " << rock->getW() << " " << rock->getH() << "\n";
            }
        }
        frame ++;
//        cout << frame << "\n";
    }

    //..............................round...........................................
    if (roundWon) {
//        cout << initTimer.timeIsUp() << "\n";
        if (initTimer.timeIsUp()) {
            if (round == BOSS_ROUND) {
//                setGameStatus(GAME_WON);
//                return;
                NG ++;
            }
//            cerr << "Start init\n";
            round = (round % ROUND_COUNT) + 1;
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
    float chicken_step_x = 0, chicken_step_y = 0;
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

    if (gundam.getLives() <= 0) {
        Text gameover("Game Over!", WHITE_COLOR_2ND);
        gameover.renderText(fontMenu, renderer, true);
        gameover.setRect(SCREEN_WIDTH/2 - gameover.getW()/2, SCREEN_HEIGHT/3);
        gameover.renderText(fontMenu, renderer);
        if (gameEndTimer.timeIsUp()) {
            setGameStatus(GAME_OVER);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}

void Game::dropUpgrade(EntityType eType) {
    if (eType != LEVEL_UP && eType != NEW_WEAPON) return;

    int x = (1ll * rand() * rand()) % SCREEN_WIDTH, y = -10;
    UpgradeType uType = (eType == LEVEL_UP ? UPGRADE_LEVEL_UP : UpgradeType(Rand(0, NUMBER_OF_UPGRADE - 2)));
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
    initTimer.startCountdown();
    score += ROUND_SCORE[round - 1] + NG_ROUND_SCORE[round - 1] * NG;
}

void Game::handleGameEvent() {
    const int gundamLaserDamage = GUNDAM_LASER_DAMAGE + gundam.getLevel() * 2;

    //.............................upgrade.....................................................
    for (Upgrade *upgrade: upgrades) {
        if (gundam.isAlive() && upgrade->getEntity()->collisionWith(*gundam.getEntity())) {
//            cout << "Level Up\n";
            if (upgrade->getType() == UPGRADE_LEVEL_UP) {
                gundam.levelUp();
            }
            else {
//                cout << upgrade->getType() << "\n";
                UpgradeType uType = upgrade->getType();
                if (uType == UPGRADE_ADD_LASER) {
//                    cout << "GET LASER\n";
                    gundamLaserTimer.startCountdown();
                }
                else {
                    gundam.addWeapon(WeaponType(uType));
                }
            }
            upgrades.erase(upgrade);
            playChunk(media->upgrade);
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
//                    chickenBullets.push_back(bullet);
//                    chicken->removeBullet(bullet);
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
                        chickenDead(chicken);
                        break;
                    }
                    else {
                        if (Rand(0, 100) < 15) playChunk(media->chickens[Rand(0, 1)]);
                    }
                }
            }

            if (gundam.isLaserOn() && chicken->getEntity()->collisionWith(gundam.getLaser())) {
                bool alive = chicken->receiveDamage(gundamLaserDamage);
                if (!alive) chickenDead(chicken);
            }
        }
    }

    //...............................rock..........................................
    for (Rock *rock: rocks) if (rock->isActive()) {
        if (gundam.isAlive() && rock->collisionWith(*gundam.getEntity())) {
            gundamDead();
        }

        set<Bullet*> gundamBullets = gundam.getBullets();
        for (Bullet *bullet: gundamBullets) {
            if (rock->collisionWith(*(bullet->getEntity()))) {
                rock->receiveDamage(gundam.getBulletDamage());
                gundam.removeBullet(bullet);
                playChunk(media->bulletRock);
            }
        }

        if (gundam.isLaserOn() && rock->collisionWith(gundam.getLaser())) {
            rock->receiveDamage(gundamLaserDamage);
            playChunk(media->bulletRock);
        }
    }

    int rockWaveCount = round == ROCK_FALL_ROUND ? ROCK_FALL_WAVE : ROCK_SIDE_WAVE + NG * NG_ROCK_WAVE;
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
    if (!gundamShieldTimer.timeIsUp()) {
        playChunk(media->bulletRock);
        return;
    }
    gundam.dead();
    addExplosion(gundam.getEntity()->getRect());
    playChunk(media->explosions[0]);
    gundamReviveTimer.startCountdown();
    hearts.pop_back();
    if (hearts.empty()) {
        gameEndTimer.startCountdown();
    }
}

void Game::chickenDead(Chicken *chicken) {
//                    chickens.erase(chicken);
    if (chicken->getLevel() == 0) {
        if (Rand(0, 100) < 20) playChunk(media->chickens[2]);
    }
    else {
        playChunk(media->explosions[1]);
    }

    addExplosion(chicken->getEntity()->getRect());
    int chickenLevel = chicken->getLevel();
    int killed = (++ killedChickenCount[chickenLevel]);
    if (chickenLevel == 0 && killed % 30 == 0) {
        dropUpgrade(LEVEL_UP);
    }
    if (chickenLevel == 1) {
        dropUpgrade(NEW_WEAPON);
    }

    numberOfAliveChicken --;
    if (numberOfAliveChicken == 0) {
        setRoundWon();
//                        cerr << "Round won\n";
    }

    score += CHICKEN_SCORE[chicken->getLevel()] + NG_CHICKEN_SCORE[chicken->getLevel()] * NG;
}

void Game::renderMenu() {
    if (status != GAME_INITALIZING) return;

    static int menuState = MENU_MAIN;

    Entity menu(MENU, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, gallery->menu);
    Entity menu_settings(MENU, {0, 0, SCREEN_WIDTH, SCREEN_WIDTH}, gallery->menu_settings);

    const int mainMenuTextCount = 4;
    Text mainMenuText[] = {
        Text("", WHITE_COLOR),
        Text("", WHITE_COLOR),
        Text("", WHITE_COLOR),
        Text("", WHITE_COLOR),
    };
    const int mainMenuChoiceCount = 4;
    Text mainMenuChoice[] = {
        Text("Start", WHITE_COLOR),
        Text("Settings", WHITE_COLOR),
        Text("Ranking", WHITE_COLOR),
        Text("Quit", WHITE_COLOR),
    };

    Text backButton("Back", WHITE_COLOR);
    backButton.renderText(fontRoundTitle, renderer);
    backButton.setRect(SCREEN_WIDTH - backButton.getW() - 100, SCREEN_HEIGHT - backButton.getH() - 50);

    const int settingsMenuTextCount = 3;
    Text settingsMenuText[] = {
        Text("", WHITE_COLOR),
        Text("Audio", WHITE_COLOR),
        Text("Difficulty", WHITE_COLOR)
    };
    const int settingsMenuChoiceCount = 3;
    Text settingsMenuChoice[] = {
        backButton,
        Text("", WHITE_COLOR),
        Text("", WHITE_COLOR),
    };

    const int rankingMenuTextCount = 0;
    const int rankingMenuChoiceCount = 1;
    Text rankingMenuChoice[] = {backButton};


    for (int i = 0; i < mainMenuChoiceCount; ++ i) {
        mainMenuChoice[i].renderText(fontMenu, renderer, true);
        mainMenuChoice[i].setRect(SCREEN_WIDTH/2 - mainMenuChoice[i].getW()/2, SCREEN_HEIGHT - 360 + i * 80);
    }
    for (int i = 1; i < settingsMenuTextCount; ++ i) {
        settingsMenuText[i].renderText(fontMenu, renderer, true);
        settingsMenuText[i].setRect(SCREEN_WIDTH/4, 150 + (i - 1) * 80);
    }

    bool menuRunning = true;
    playMusic(media->music);
    Text *texts, *choices;
    while (menuRunning) {
        int textCount, choiceCount;
        for (int i = 1; i < settingsMenuChoiceCount; ++ i) {
            if (i == SETTING_MENU_AUDIO) {
                settingsMenuChoice[i].setText(GAME_AUDIO[audioState]);
            }
            if (i == SETTING_MENU_DIFFICULTY) {
                settingsMenuChoice[i].setText(GAME_DIFFICULTY[difficultyState]);
            }
            settingsMenuChoice[i].renderText(fontMenu, renderer, true);
            settingsMenuChoice[i].setRect(SCREEN_WIDTH*3/4 - settingsMenuChoice[i].getW()/2, 150 + (i - 1) * 80);
        }
//        cout << textCount << " " << choiceCount << "\n";

        if (menuState == MENU_MAIN) {
            texts = mainMenuText;
            choices = mainMenuChoice;
            textCount = mainMenuTextCount;
            choiceCount = mainMenuChoiceCount;
            menu.render(renderer);
        }
        else if (menuState == MENU_SETTINGS) {
            texts = settingsMenuText;
            choices = settingsMenuChoice;
            textCount = settingsMenuTextCount;
            choiceCount = settingsMenuChoiceCount;
            menu_settings.render(renderer);
        }
        else {
            textCount = rankingMenuTextCount;
            choiceCount = rankingMenuChoiceCount;
            choices = rankingMenuChoice;
            menu_settings.render(renderer);
            showRanking();
        }

        for (int i = 0; i < textCount; ++ i) {
            if (menuState == MENU_MAIN) continue;
            texts[i].renderText(fontMenu, renderer);
        }
        for (int i = 0; i < choiceCount; ++ i) {
            choices[i].renderText(fontMenu, renderer);
        }


        while (SDL_PollEvent(event) != 0) {
            if (event->type == SDL_QUIT) {
                setGameStatus(GAME_STOP);
                menuRunning = false;
            }
            else if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < choiceCount; ++ i) {
                    if (choices[i].getX() <= (event->motion).x && (event->motion).x <= choices[i].getX() + choices[i].getW() && choices[i].getY() <= (event->motion).y && (event->motion).y <= choices[i].getY() + choices[i].getH()) {
                        if (event->type == SDL_MOUSEMOTION) {
                            choices[i].setColor(RED_COLOR);
                        }
                        else {
                            playChunk(media->upgrade);
                            if (menuState == MENU_MAIN) {
                                if (i == MAIN_MENU_START) {
                                    setGameStatus(GAME_PLAYING);
                                    initTimer.startCountdown();
                                    menuRunning = false;
                                }
                                if (i == MAIN_MENU_SETTINGS) {
                                    menuState = MENU_SETTINGS;
                                }
                                if (i == MAIN_MENU_RANKING) {
                                    menuState = MENU_RANKING;
                                }
                                if (i == MAIN_MENU_QUIT) {
                                    setGameStatus(GAME_STOP);
                                    menuRunning = false;
                                }
                            }
                            else {
                                if (i == SETTING_MENU_AUDIO) {
                                    audioState = GameAudio((audioState + 1) % AUDIO_COUNT);
                                    if (audioState == AUDIO_MUTED) {
                                        Mix_PauseMusic();
                                    }
                                    else {
                                        Mix_ResumeMusic();
                                    }
                                }
                                else if (i == SETTING_MENU_DIFFICULTY) {
                                    difficultyState = GameDifficulty((difficultyState + 1) % GAME_DIFFICULTY_COUNT);
                                }
                                else {
                                    menuState = MENU_MAIN;
                                }
                            }
                        }
                    }
                    else {
                        choices[i].setColor(WHITE_COLOR);
                    }
                }
            }
        }

        SDL_RenderPresent(renderer);
    }
    Mix_PauseMusic();
}

void Game::initData() {
    score = round = NG = 0;
    frame = scrolling = 0;
    roundWon = true;

    killedChickenCount.assign(5, 0);
    chickens.clear(); chickenBullets.clear();
    rocks.clear();
    upgrades.clear();
    explosions.clear();
    hearts.clear();

    for (int i = 0; i < gundam.getLives(); ++ i) {
        hearts.push_back(Entity(HEART, {10 + i * 27, 10, 25, 25}));
        hearts.back().setTexture(gallery->heart);
    }
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
    TTF_Init();
    fontMenu = TTF_OpenFont("./assets/font/Zebulon Bold.otf", 50);
    fontGame = TTF_OpenFont("./assets/font/Zebulon.otf", 24);
    fontRoundTitle = TTF_OpenFont("./assets/font/Zebulon Bold.otf", 32);
    fontRoundText = TTF_OpenFont("./assets/font/Zebulon.otf", 20);

    initData();

    scoreText.setText("Score: ");
    scoreText.setColor(WHITE_COLOR);
    scoreText.renderText(fontGame, renderer);
    scoreText.setRect(10, 10 + hearts.back().getH() + 5);
    scoreValue.setText("0");
    scoreValue.setColor(WHITE_COLOR);
    scoreValue.renderText(fontRoundText, renderer);
//    cout << scoreText.getW() << "\n";
    scoreValue.setRect(10 + scoreText.getW(), 10 + hearts.back().getH() + 5 + scoreText.getH() - scoreValue.getH());

//    setGameStatus(GAME_RUNNING);

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
    if (audioState == AUDIO_MUTED) return;
    Mix_PlayMusic(music, -1);
}

bool isHover(const SDL_Event &event, const Entity &entity) {
    return entity.getX() <= event.motion.x && event.motion.x <= entity.getX() + entity.getW() && entity.getY() <= event.motion.y && event.motion.y <= entity.getY() + entity.getH();
}

void Game::gameOver() {
    if (status != GAME_OVER) return;

    enterYourName();
    if (!isRunning()) return;

    Text nextButton("Next", WHITE_COLOR);
    nextButton.renderText(fontRoundTitle, renderer, true);
    nextButton.setRect(SCREEN_WIDTH - nextButton.getW() - 100, SCREEN_HEIGHT - nextButton.getH() - 50);

    SDL_Event e;
    bool rankShowing = true;
    while (rankShowing) {
        (scrolling += SCREEN_SPEED) %= BG_SIZE;
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
                        nextButton.setColor(RED_COLOR);
                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        playChunk(media->upgrade);
                        rankShowing = false;
                        break;
                    }
                }
                else {
                    nextButton.setColor(WHITE_COLOR);
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

    Text placeholder("Enter your name!", WHITE_COLOR_2ND);
    placeholder.renderText(fontMenu, renderer, true);
    placeholder.setRect(SCREEN_WIDTH/2 - placeholder.getW()/2, SCREEN_HEIGHT/3 + 50);

    Text input("", WHITE_COLOR);
    input.setRect(SCREEN_WIDTH/2 - placeholder.getW()/2, SCREEN_HEIGHT/3);
    string username = "";

    Text nextButton("Next", WHITE_COLOR);
    nextButton.renderText(fontRoundTitle, renderer, true);
    nextButton.setRect(SCREEN_WIDTH/2 - nextButton.getW()/2, SCREEN_HEIGHT/3 + 150);

    bool summarizing = true;
    SDL_Event e;
    while (summarizing) {
        (scrolling += SCREEN_SPEED) %= BG_SIZE;
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
                        nextButton.setColor(RED_COLOR);
                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        summarizing = false;
                        playChunk(media->upgrade);
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
                    nextButton.setColor(WHITE_COLOR);
                }
            }
            if (e.type == SDL_KEYDOWN) {
                playChunk(media->bulletRock);
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
        playerNames[i].setColor(WHITE_COLOR);
        playerScores[i].setColor(WHITE_COLOR);
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

    gundamLaserTimer.deactive();
    gundamReviveTimer.deactive();
    gundamShieldTimer.deactive();
    initTimer.deactive();

    gundam.reset();

    initData();

    scoreText.setRect(10, 10 + hearts.back().getH() + 5);
    scoreValue.setRect(10 + scoreText.getW(), 10 + hearts.back().getH() + 5 + scoreText.getH() - scoreValue.getH());
}
