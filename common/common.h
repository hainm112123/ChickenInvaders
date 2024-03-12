#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <set>
#include <vector>
#include <assert.h>
#include <chrono>
#include <thread>
#include <ctime>
#include <deque>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <iostream>

#include <SDL.h>

#define CLOCK_NOW chrono::system_clock::now
#define _size(x) (int((x).size()))

using namespace std;

typedef chrono::system_clock::time_point Time;
typedef chrono::duration<double> ElapsedTime;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 4096;

const int NUMBER_OF_EXPLOSION_PIC = 8;
const int FRAME_PER_PICTURE = 5;
const int FRAME_PER_PICTURE_MORE = 8;

const int GUNDAM_LASER_WIDTH = 80;
const int GUNDAM_LASER_HIGHT = 682;
const int GUNDAM_LASER_DAMAGE = 3;

int Rand(int l, int r);

struct Texture {
    SDL_Texture *texture;
    int w, h;
};

#endif // INIT_H_INCLUDED
