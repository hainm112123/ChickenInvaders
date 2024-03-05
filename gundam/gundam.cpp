#include "gundam.h"
#include "../weapon/bullet.h"

Gundam::Gundam(): entity(GUNDAM, {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, GUNDAM_WIDTH, GUNDAM_HEIGHT}) {
    lives = 3;
    alive = true;
    weapons.push_back(GUNDAM_BLASTER);
//    weapons.push_back(GUNDAM_BORON);
//    weapons.push_back(GUNDAM_NEUTRON);
    level = 0;
    currentWeaponID = 0;
}

void Gundam::render(SDL_Renderer *renderer, Gallery *gallery) {
    if (!alive) return;
    entity.setTexture(gallery->gundams[getCurrentWeapon()]);
    entity.render(renderer);
}

void Gundam::_move() {
    if (!alive) return;
    entity._move(true);
}

void Gundam::control(SDL_Event event, Gallery *gallery) {
    for (int type = GUNDAM_MOVE_UP; type <= GUNDAM_MOVE_RIGHT; type += 1) {
        if (MoveKeyCode[type] == event.key.keysym.sym) {
            if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                entity.updateStep(gundam_step_x[type] * GUNDAM_SPEED, gundam_step_y[type] * GUNDAM_SPEED);
//                cout << "Key Down\n";
            }
            else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
                entity.updateStep(-gundam_step_x[type] * GUNDAM_SPEED, -gundam_step_y[type] * GUNDAM_SPEED);
//                cout << "Key Up\n";
            }
        }
    }
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        if (event.key.keysym.sym == SDLK_SPACE && alive) {
            Bullet *bullet = new Bullet();
            Texture texture = gallery->gundamWeapons[getCurrentWeapon()][level];
            bullet->setEntity({entity.getX() + entity.getW() / 2 - texture.w / 2, entity.getY() - texture.h, texture.w, texture.h}, -GUNDAM_BULLET_SPEED[getCurrentWeapon()], texture);
            bullet->setIsMove(true);
            bullets.insert(bullet);
        }
        if (event.key.keysym.sym == SDLK_e && !alive) {
            revive();
//            cout << alive << " " << entity.getX() << " " << entity.getY() << "\n";
        }
        if (event.key.keysym.sym == SDLK_q && alive) {
            changeWeapon();
        }
    }
}

void Gundam::handleBullet(SDL_Renderer *renderer) {
    for (Bullet *bullet: bullets) {
        if (bullet->getIsMove()) {
            bullet->render(renderer);
            bullet->handleGundamBullet();
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
    return GUNDAM_BULLET_DAMAGE[getCurrentWeapon()];
}

void Gundam::levelUp() {
    level = min(level + 1, 3);
}
