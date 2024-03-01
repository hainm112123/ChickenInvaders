#include "gundam.h"
#include "../weapon/bullet.h"

Gundam::Gundam(): entity({SCREEN_WIDTH / 2, SCREEN_HEIGHT - GUNDAM_SIZE, GUNDAM_SIZE, GUNDAM_SIZE}) {
    bulletDmg = GUNDAM_BLASTER_DAMAGE;
    lives = 3;
    alive = true;
}

void Gundam::render(SDL_Renderer *renderer, Painter *painter) {
    if (!alive) return;
    painter->setColor(GREEN_COLOR);
    entity.render(renderer);
}

void Gundam::_move() {
    if (!alive) return;
    entity._move(true);
}

void Gundam::control(SDL_Event event) {
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
            Bullet *bullet = new Bullet(Entity({entity.getX() + entity.getW() / 2 - BLASTER_WIDTH / 2, entity.getY() - BLASTER_HEIGHT, BLASTER_WIDTH, BLASTER_HEIGHT}));
            bullet->getEntity()->setStep(0, -GUNDAM_BLASTER_SPEED);
            bullet->setIsMove(true);
            bullets.insert(bullet);
        }
        if (event.key.keysym.sym == SDLK_e && !alive) {
            revive();
//            cout << alive << " " << entity.getX() << " " << entity.getY() << "\n";
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
