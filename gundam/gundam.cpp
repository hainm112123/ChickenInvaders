#include "gundam.h"
#include "../game/game.h"
#include "../weapon/bullet.h"

Gundam::Gundam():
    entity(GUNDAM, {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, GUNDAM_WIDTH, GUNDAM_HEIGHT}),
    shield(SHIELD), laser(LASER),
    reviveTimer(GUNDAM_REVIVE_TIME), shieldTimer(GUNDAM_SHIELD_DURATION), laserTimer(GUNDAM_LASER_DURATION)
{
    lives = GUNDAM_LIVES;
    alive = true;
    weapons.push_back(GUNDAM_BLASTER);
//    weapons.push_back(GUNDAM_BORON);
//    weapons.push_back(GUNDAM_NEUTRON);
//    weapons.push_back(GUNDAM_AUTO_AIM);
    level = bullet_form = 0;
    currentWeaponID = 0;
    appearance = GUNDAM_CENTER;
    moving_state = GUNDAM_STAY;
    turned_time = 0;

//    entity.setTexture(Gallery::Instance()->gundams[getCurrentWeapon()], true);
    entity.setTextures(Gallery::Instance()->gundams[appearance]);
    shield.setTexture(Gallery::Instance()->shield);
    int shieldSize = max(entity.getW(), entity.getH()) + 40;
//    cout << entity.getX() << " " << entity.getW() << " " << shieldSize << "\n";
    shield.setRect({entity.getX() + entity.getW()/2 - shieldSize/2, entity.getY() + entity.getH()/2 - shieldSize/2, shieldSize, shieldSize});
    laser.setTexture(Gallery::Instance()->laser);
    laser.setRect({entity.getX() + entity.getW()/2 - GUNDAM_LASER_WIDTH/2, entity.getY() - GUNDAM_LASER_HIGHT, GUNDAM_LASER_WIDTH, GUNDAM_LASER_HIGHT});

    fire_timer.setDuration(FIRE_DELAY[getCurrentWeapon()]);

    resetControl();
}

Gundam::~Gundam() {
    for (Bullet *bullet: bullets) delete(bullet);
    bullets.clear();
}

void Gundam::setGame(Game *_game) {
    game = _game;
}

void Gundam::render(SDL_Renderer *renderer, bool hasShield, bool hasLaser) {
    if (!alive) return;
//    entity.setTexture(Gallery::Instance()->gundams[getCurrentWeapon()]);
    if (turned_time < GUNDAM_APPEARANCE_CHANGE) turned_time += TimeManager::Instance()->getElapsedTime();
    if (moving_state == GUNDAM_STAY) {
        appearance = GUNDAM_CENTER;
    }
    else if (moving_state == GUNDAM_MOVING_LEFT) {
        appearance = turned_time < GUNDAM_APPEARANCE_CHANGE ? GUNDAM_LEFT_1 : GUNDAM_LEFT_2;
    }
    else {
        appearance = turned_time < GUNDAM_APPEARANCE_CHANGE ? GUNDAM_RIGHT_1 : GUNDAM_RIGHT_2;
    }
//    cout << turned_time << " " << appearance << "\n";

    entity.setTextures(Gallery::Instance()->gundams[appearance]);
    entity.render(renderer);
    if (hasShield) shield.render(renderer);
    if (hasLaser && laserOn) laser.render(renderer);
}

void Gundam::_move() {
    fire_timer.process();

    if (!alive) return;
    entity._move(true);
    int shieldSize = max(entity.getW(), entity.getH()) + 60;
    shield.setRect({entity.getX() + entity.getW()/2 - shieldSize/2, entity.getY() + entity.getH()/2 - shieldSize/2, shieldSize, shieldSize});
    laser.setRect({entity.getX() + entity.getW()/2 - GUNDAM_LASER_WIDTH/2, entity.getY() - GUNDAM_LASER_HIGHT, GUNDAM_LASER_WIDTH, GUNDAM_LASER_HIGHT});
//    shield._move();
}

void Gundam::control(SDL_Event event) {
    for (int type = GUNDAM_MOVE_UP; type <= GUNDAM_MOVE_RIGHT; type += 1) {
//        if (type == GUNDAM_MOVE_DOWN || type == GUNDAM_MOVE_UP) continue;
        if (MoveKeyCode[type] == event.key.keysym.sym) {
            if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
//                entity.setStep(gundam_step_x[type] * GUNDAM_SPEED, gundam_step_y[type] * GUNDAM_SPEED);
                entity.updateStep(GUNDAM_SPEED * gundam_step_x[type], GUNDAM_SPEED * gundam_step_y[type]);
//                shield.updateStep(gundam_step_x[type] * GUNDAM_SPEED, gundam_step_y[type] * GUNDAM_SPEED);
//                cout << "Key Down\n";
                keydown[type] = 1;

                if (type == GUNDAM_MOVE_LEFT && moving_state != GUNDAM_MOVING_LEFT) {
                    turned_time = 0;
                    moving_state = GUNDAM_MOVING_LEFT;
                }
                if (type == GUNDAM_MOVE_RIGHT && moving_state != GUNDAM_MOVING_RIGHT) {
                    turned_time = 0;
                    moving_state = GUNDAM_MOVING_RIGHT;
                }
            }
            else if (event.type == SDL_KEYUP && event.key.repeat == 0 && keydown[type]) {
//                entity.setStep(0, 0);
                entity.updateStep(-GUNDAM_SPEED * gundam_step_x[type], -GUNDAM_SPEED * gundam_step_y[type]);
//                shield.updateStep(-gundam_step_x[type] * GUNDAM_SPEED, -gundam_step_y[type] * GUNDAM_SPEED);
//                cout << "Key Up\n";
                keydown[type] = 0;

                if (type == GUNDAM_MOVE_LEFT && moving_state == GUNDAM_MOVING_LEFT) {
                    turned_time = 0;
                    moving_state = GUNDAM_STAY;
                }
                if (type == GUNDAM_MOVE_RIGHT && moving_state == GUNDAM_MOVING_RIGHT) {
                    turned_time = 0;
                    moving_state = GUNDAM_STAY;
                }
            }
        }
    }
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        if (event.key.keysym.sym == SDLK_SPACE && alive && laserTimer.timeIsUp() && fire_timer.timeIsUp()) {
            fire_timer.startCountdown();

            Bullet *bullet = new Bullet(getCurrentWeapon());
            bullet_form = min(level, NUMBER_OF_BULLET_FORM - 1);
//            cout << level << "\n";
            Texture texture = Gallery::Instance()->gundamWeapons[getCurrentWeapon()][bullet_form];
            bullet->setEntity({entity.getX() + entity.getW() / 2 - texture.w / 2, entity.getY() - texture.h, texture.w, texture.h}, -GUNDAM_BULLET_SPEED[getCurrentWeapon()], texture);
            bullet->setIsMove(true);
            bullets.insert(bullet);
            game->playChunk(Media::Instance()->bullets[getCurrentWeapon()]);
        }
//        if (event.key.keysym.sym == SDLK_e && !alive) {
//            revive();
////            cout << alive << " " << entity.getX() << " " << entity.getY() << "\n";
//        }
        if (event.key.keysym.sym == SDLK_q && alive) {
            changeWeapon();
        }
    }
    if (!laserTimer.timeIsUp() && event.key.keysym.sym == SDLK_SPACE && alive) {
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
            setLaserOn(true);
            keydown[GUNDAM_FIRE] = 1;
        }
        if (event.type == SDL_KEYUP && event.key.repeat == 0 && keydown[GUNDAM_FIRE]) {
            setLaserOn(false);
            keydown[GUNDAM_FIRE] = 0;
        }
    }
}

void Gundam::handleBullet(SDL_Renderer *renderer, const vector<pair<double, double>> &enemy_positions) {
    for (Bullet *bullet: bullets) {
        if (bullet->getIsMove()) {
            bullet->render(renderer);
            double target_x = -oo, target_y = -oo;
            double min_dist = oo;
//            cout << bullet->Type() << "\n";
            if (bullet->Type() == GUNDAM_AUTO_AIM) {
                for (auto &pos: enemy_positions) {
                    double current_dist = dist(pos.first, pos.second, bullet->getEntity()->get_act_x(), bullet->getEntity()->get_act_y());
                    if (current_dist < min_dist) {
                        target_x = pos.first;
                        target_y = pos.second;
                        min_dist = current_dist;
                    }
//                    cout << pos.first << " " << pos.second << "\n";
                }
            }
            bullet->handleGundamBullet(target_x, target_y);
        }
        else {
            bullets.erase(bullet);
        }
    }
}

void Gundam::removeBullet(Bullet *bullet) {
    assert(bullets.find(bullet) != bullets.end());
    bullets.erase(bullet);
    delete(bullet);
}

void Gundam::dead() {
    setLaserOn(false);
    alive = false;
    lives --;
    level = max(level - 1, 0);
//    cout << "dead " << lives << "\n" ;
}
bool Gundam::revive() {
//    cout << lives << "\n";
    if (lives > 0) {
        alive = true;
        return true;
    }
    return false;
}

void Gundam::addWeapon(WeaponType newWeapon) {
//    cout << "Add new weapon\n";
    for (int i = 0; i < int(weapons.size()); ++ i) if (weapons[i] == newWeapon) {
//        cout << "Already had\n";
        currentWeaponID = i;
        levelUp();
//        cout << level << "\n";
        return;
    }
    weapons.push_back(newWeapon);
    currentWeaponID = int(weapons.size()) - 1;
//    cout << "New weapon added\n";

    fire_timer.deactive();
    fire_timer.setDuration(FIRE_DELAY[getCurrentWeapon()]);
}

void Gundam::changeWeapon() {
    currentWeaponID = (currentWeaponID + 1) % (int(weapons.size()));

    fire_timer.deactive();
    fire_timer.setDuration(FIRE_DELAY[getCurrentWeapon()]);
}

int Gundam::getBulletDamage() {
    return GUNDAM_BULLET_DAMAGE[getCurrentWeapon()] + level * GUNDAM_BULLET_UPGRADE[getCurrentWeapon()];
}

void Gundam::levelUp() {
    level = min(level + 1, MAX_GUNDAM_LEVEL);
}

void Gundam::setLaserOn(bool _laserOn) {
    laserOn = _laserOn;
    if (!laserOn) {
        Mix_HaltChannel(7);
    }
    else {
//        cout << "laser chunk\n";
        game->playChunk(Media::Instance()->laser, 7, -1);
    }
}

void Gundam::reset() {
    lives = 5;
    alive = true;
    weapons.clear();
    bullets.clear();
    weapons.push_back(GUNDAM_BLASTER);
    level = bullet_form = 0;
    currentWeaponID = 0;
    laserOn = false;

    entity.setRect(SCREEN_WIDTH/2, SCREEN_HEIGHT - 100);
    entity.setStep(0, 0);
    int shieldSize = max(entity.getW(), entity.getH()) + 20;
    shield.setRect({entity.getX() + entity.getW()/2 - shieldSize/2, entity.getY() + entity.getH()/2 - shieldSize/2, shieldSize, shieldSize});
    laser.setRect({entity.getX() + entity.getW()/2 - GUNDAM_LASER_WIDTH/2, entity.getY() - GUNDAM_LASER_HIGHT, GUNDAM_LASER_WIDTH, GUNDAM_LASER_HIGHT});

    resetControl();
}

void Gundam::resetControl() {
    appearance = GUNDAM_CENTER;
    moving_state = GUNDAM_STAY;
    turned_time = 0;

    entity.setStep(0, 0);
    setLaserOn(false);
    memset(keydown, 0, sizeof(keydown));
}

void Gundam::processTimer() {
    reviveTimer.process();
    shieldTimer.process();
    laserTimer.process();
}

void Gundam::deactiveTimer() {
    laserTimer.deactive();
    reviveTimer.deactive();
    shieldTimer.deactive();
}
