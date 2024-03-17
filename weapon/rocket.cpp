#include "rocket.h"

Rocket::Rocket(double _src_x, double _src_y, double _dst_x, double _dst_y):
    src_x(_src_x), src_y(_src_y), dst_x(_dst_x), dst_y(_dst_y),
    explosion(BIG_EXPLOSION)
{
    type = ROCKET;
    x = src_x; y = src_y;
    rect = {int(x), int(y), ROCKET_WIDTH, ROCKET_HEIGHT};
    R = dist(src_x, src_y, dst_x, dst_y)/2;
    distance = R * M_PI;
    center_x = (src_x + dst_x) / 2;
    center_y = (src_y + dst_y) / 2;
    setTexture(Gallery::Instance()->rocket);

    explosion.setRect({int(dst_x - BIG_EXPLOSION_WIDTH/2), int(dst_y - BIG_EXPLOSION_HEIGHT/2), BIG_EXPLOSION_WIDTH, BIG_EXPLOSION_HEIGHT});
    explosion.setTexture(Gallery::Instance()->big_explosion);
}

Rocket::~Rocket() {}

bool Rocket::reached(bool is_audio_on) {
    if (!active) return false;
    if (abs(x - dst_x) < 2 && abs(y - dst_y) < 2) {
        active = false;
        exp = true;
        if (is_audio_on) {
            Mix_PlayChannel(-1, Media::Instance()->explosions[1], 0);
        }
        return true;
    }
    return false;
}

void Rocket::handleMove() {
    if (!active) return;
//    double static L = 0;
//    cout << x << " " << y << "\n";
    double elapsed = (TimeManager::Instance()->getElapsedTime());
    double lent = distance * elapsed / MOVE_TIME;
//    L += lent;
//    cout << L << " " << distance << "\n";
//    cout << x << " " << y << " " << dst_x << " " << dst_y << "\n";
    double angle = to_radian(lent * 180 / (R * M_PI));
    double u_x = x - center_x, u_y = y - center_y;
    double v_x = u_x * cos(angle) - u_y * sin(angle);
    double v_y = u_x * sin(angle) + u_y * cos(angle);
    double next_x = center_x + v_x, next_y = center_y + v_y;
//    setPosition(next_x, next_y);
    setStep((next_x - x) / elapsed, (next_y - y) / elapsed);
    _move();
}

void Rocket::handleExplosion(SDL_Renderer *renderer) {
    if (!exp) return;
    if (explosion.CurrentTime() >= SECOND_PER_PICTURE_LONGER * NUMBER_OF_BIG_EXPLOSION_PIC) {
        exp = false;
        return;
    }
    explosion.render(renderer);
}

void Rocket::Set() {
    active = true;
    exp = false;
    explosion.resetTime();
    x = src_x;
    y = src_y;
}
