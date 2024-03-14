#include "bullet.h"

Bullet::Bullet(WeaponType _type): type(_type) {
    isMove = false;
}
Bullet::Bullet(Entity _entity, WeaponType _type): entity(_entity), type(_type) {
    isMove = false;
}

void Bullet::setIsMove(const bool _isMove) {
    isMove = _isMove;
}
void Bullet::setEntity(SDL_Rect rect, double _speed, Texture texture) {
    speed = _speed;
    entity = Entity(BULLET, rect);
    entity.setStep(0, speed);
    entity.setTexture(texture);
//    cout << entity.getX() << " " << entity.getY() << " " << speed << "\n";
}

void Bullet::render(SDL_Renderer *renderer) {
//    cout << entity.getX() << " " << entity.getY() << "\n";
    entity.render(renderer, 1);
}

void Bullet::handleGundamBullet(double target_x, double target_y) {
//    if (target_x != -oo && target_y != -oo) {
//        cout << target_x << " " << target_y << " " << entity.get_act_x() << " " << entity.get_act_y() << "\n";
//        cout << dist(target_x, target_y, entity.get_act_x(), entity.get_act_y()) << "\n";
//    }
    if (target_x != -oo && target_y != -oo && dist(target_x, target_y, entity.get_act_x(), entity.get_act_y()) < MAX_AUTO_AIM_DISTANCE) {
        double dist = sqrt(sqr(target_x - entity.getX()) + sqr(target_y - entity.getY()));
        double v_x = abs(speed) * (target_x - entity.getX()) / dist;
        double v_y = abs(speed) * (target_y - entity.getY()) / dist;
        entity.setStep(v_x, v_y);
//        cout << v_x << " " << v_y << " " << sqrt(sqr(v_x) + sqr(v_y))<< "\n";
    }
    entity._move();
    if (entity.getY() > SCREEN_HEIGHT + 100 || entity.getY() < -entity.getH() - 100 || entity.getX() < -entity.getW() - 100 || entity.getX() > SCREEN_WIDTH + 100) {
        setIsMove(false);
    }
}

void Bullet::handleEnemyBullet() {
    entity._move();
    if (entity.getY() > SCREEN_HEIGHT + entity.getH() + 100) {
        setIsMove(false);
    }
}

