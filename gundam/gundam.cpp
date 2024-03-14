#include "gundam.h"
#include "../game/game.h"
#include "../weapon/bullet.h"

Gundam::Gundam(Gallery *gallery): entity(GUNDAM, {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, GUNDAM_WIDTH, GUNDAM_HEIGHT}), shield(SHIELD), laser(LASER) {
    lives = 3;
    alive = true;
    weapons.push_back(GUNDAM_BLASTER);
//    weapons.push_back(GUNDAM_AUTO_AIM);
//    weapons.push_back(GUNDAM_BORON);
//    weapons.push_back(GUNDAM_NEUTRON);
    level = 0;
    currentWeaponID = 0;

    entity.setTexture(gallery->gundams[getCurrentWeapon()], true);
    shield.setTexture(gallery->shield);
    int shieldSize = max(entity.getW(), entity.getH()) + 20;
//    cout << entity.getX() << " " << entity.getW() << " " << shieldSize << "\n";
    shield.setRect({entity.getX() + entity.getW()/2 - shieldSize/2, entity.getY() + entity.getH()/2 - shieldSize/2, shieldSize, shieldSize});
    laser.setTexture(gallery->laser);
    laser.setRect({entity.getX() + entity.getW()/2 - GUNDAM_LASER_WIDTH/2, entity.getY() - GUNDAM_LASER_HIGHT, GUNDAM_LASER_WIDTH, GUNDAM_LASER_HIGHT});

    memset(keydown, 0, sizeof(keydown));
}

void Gundam::setGame(Game *_game) {
    game = _game;
}

void Gundam::render(SDL_Renderer *renderer, Gallery *gallery, bool hasShield, bool hasLaser) {
    if (!alive) return;
    entity.setTexture(gallery->gundams[getCurrentWeapon()]);
    entity.render(renderer);
    if (hasShield) shield.render(renderer);
    if (hasLaser && laserOn) laser.render(renderer);
}

void Gundam::_move() {
    if (!alive) return;
    entity._move(true);
    int shieldSize = max(entity.getW(), entity.getH()) + 20;
    shield.setRect({entity.getX() + entity.getW()/2 - shieldSize/2, entity.getY() + entity.getH()/2 - shieldSize/2, shieldSize, shieldSize});
    laser.setRect({entity.getX() + entity.getW()/2 - GUNDAM_LASER_WIDTH/2, entity.getY() - GUNDAM_LASER_HIGHT, GUNDAM_LASER_WIDTH, GUNDAM_LASER_HIGHT});
//    shield._move();
}

void Gundam::control(SDL_Event event, Timer &gundamLaserTimer) {
    for (int type = GUNDAM_MOVE_UP; type <= GUNDAM_MOVE_RIGHT; type += 1) {
//        if (type == GUNDAM_MOVE_DOWN || type == GUNDAM_MOVE_UP) continue;
        if (MoveKeyCode[type] == event.key.keysym.sym) {
            if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
//                entity.setStep(gundam_step_x[type] * GUNDAM_SPEED, gundam_step_y[type] * GUNDAM_SPEED);
                entity.updateStep(GUNDAM_SPEED * gundam_step_x[type], GUNDAM_SPEED * gundam_step_y[type]);
//                shield.updateStep(gundam_step_x[type] * GUNDAM_SPEED, gundam_step_y[type] * GUNDAM_SPEED);
//                cout << "Key Down\n";
                keydown[type] = 1;
            }
            else if (event.type == SDL_KEYUP && event.key.repeat == 0 && keydown[type]) {
//                entity.setStep(0, 0);
                entity.updateStep(-GUNDAM_SPEED * gundam_step_x[type], -GUNDAM_SPEED * gundam_step_y[type]);
//                shield.updateStep(-gundam_step_x[type] * GUNDAM_SPEED, -gundam_step_y[type] * GUNDAM_SPEED);
//                cout << "Key Up\n";
                keydown[type] = 0;
            }
        }
    }
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        if (event.key.keysym.sym == SDLK_SPACE && alive && gundamLaserTimer.timeIsUp()) {
            Bullet *bullet = new Bullet(getCurrentWeapon());
            Texture texture = game->getGallery()->gundamWeapons[getCurrentWeapon()][level];
            bullet->setEntity({entity.getX() + entity.getW() / 2 - texture.w / 2, entity.getY() - texture.h, texture.w, texture.h}, -GUNDAM_BULLET_SPEED[getCurrentWeapon()], texture);
            bullet->setIsMove(true);
            bullets.insert(bullet);
            game->playChunk(game->getMedia()->bullets[getCurrentWeapon()]);
        }
//        if (event.key.keysym.sym == SDLK_e && !alive) {
//            revive();
////            cout << alive << " " << entity.getX() << " " << entity.getY() << "\n";
//        }
        if (event.key.keysym.sym == SDLK_q && alive) {
            changeWeapon();
        }
    }
    if (!gundamLaserTimer.timeIsUp() && event.key.keysym.sym == SDLK_SPACE && alive) {
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
            setLaserOn(true);
        }
        if (event.type == SDL_KEYUP && event.key.repeat == 0) {
            setLaserOn(false);
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
        return;
    }
    weapons.push_back(newWeapon);
    currentWeaponID = int(weapons.size()) - 1;
//    cout << "New weapon added\n";
}

void Gundam::changeWeapon() {
    currentWeaponID = (currentWeaponID + 1) % (int(weapons.size()));
}

int Gundam::getBulletDamage() {
    return GUNDAM_BULLET_DAMAGE[getCurrentWeapon()] + level * GUNDAM_BULLET_UPGRADE[getCurrentWeapon()];
}

void Gundam::levelUp() {
    level = min(level + 1, 3);
}

void Gundam::setLaserOn(bool _laserOn) {
    laserOn = _laserOn;
    if (!laserOn) {
        Mix_HaltChannel(10000);
    }
    else {
        game->playChunk(game->getMedia()->laser, 10000, -1);
    }
}

void Gundam::reset() {
    lives = 3;
    alive = true;
    weapons.clear();
    bullets.clear();
    weapons.push_back(GUNDAM_BLASTER);
    level = 0;
    currentWeaponID = 0;
    laserOn = false;

    entity.setRect(SCREEN_WIDTH/2, SCREEN_HEIGHT - 100);
    entity.setStep(0, 0);
    int shieldSize = max(entity.getW(), entity.getH()) + 20;
    shield.setRect({entity.getX() + entity.getW()/2 - shieldSize/2, entity.getY() + entity.getH()/2 - shieldSize/2, shieldSize, shieldSize});
    laser.setRect({entity.getX() + entity.getW()/2 - GUNDAM_LASER_WIDTH/2, entity.getY() - GUNDAM_LASER_HIGHT, GUNDAM_LASER_WIDTH, GUNDAM_LASER_HIGHT});

    memset(keydown, 0, sizeof(keydown));
}
