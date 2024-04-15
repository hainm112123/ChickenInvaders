#include "common.h"

int Rand(int l, int r) {
    return l + (rand() % (r - l + 1));
}

double dist(double x, double y, double u, double v) {
    return sqrt(sqr(x - u) + sqr(y - v));
}

double to_degree(double radian) {
    return radian * 180 / M_PI;
}

double to_radian(double degree) {
    return degree * M_PI / 180;
}

double get_angle_from_step(double x, double y) {
    double deg = atan2(x, -y) * 180 / M_PI;
    return deg;
}

void setColor(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
