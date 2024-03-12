#include "common.h"

int Rand(int l, int r) {
    return l + (rand() % (r - l + 1));
}
