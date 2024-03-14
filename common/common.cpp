#include "common.h"

int Rand(int l, int r) {
    return l + (rand() % (r - l + 1));
}

double dist(double x, double y, double u, double v) {
    return sqrt(sqr(x - u) + sqr(y - v));
}
