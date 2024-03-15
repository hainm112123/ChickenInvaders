#include "rocket.h"

Rocket::Rocket(double _src_x, double _src_y, double _dst_x, double _dst_y):
    src_x(_src_x), src_y(_src_y), dst_x(_dst_x), dst_y(_dst_y)
{
    type = ROCKET;
    x = src_x; y = src_y;
    rect = {int(x), int(y), ROCKET_WIDTH, ROCKET_HEIGHT};
    R = dist(src_x, src_y, dst_x, dst_y)/2;
    distance = R * M_PI;
    center_x = (src_x + dst_x) / 2;
    center_y = (src_y + dst_y) / 2;
}

Rocket::~Rocket() {}

bool Rocket::reached() {
    return abs(x - dst_x) < 2 && abs(y - dst_y) < 2;
}

void Rocket::handleMove() {
    if (reached()) return;
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

void Rocket::reset() {
    x = src_x;
    y = src_y;
}
