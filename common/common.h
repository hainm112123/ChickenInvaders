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
#include <cmath>
#include <valarray>

#include <SDL.h>

//#define CLOCK_NOW chrono::system_clock::now
#define _size(x) (int((x).size()))
#define sqr(x) ((x) * (x))

using namespace std;

//typedef chrono::system_clock::time_point Time;
//typedef chrono::duration<double> ElapsedTime;

const int oo = int(1e9);
const double eps = 1e-9;

const SDL_Color WHITE_COLOR = {255, 255, 255, 255};
const SDL_Color WHITE_COLOR_2ND = {255, 255, 255, 0};
const SDL_Color BLACK_COLOR = {0, 0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0, 0};
const SDL_Color RED_COLOR = {255, 0, 0, 0};
const SDL_Color TEXT_COLOR = {255, 255, 255, 0};
const SDL_Color TEXT_HOVER_COLOR = {128, 128, 128, 0};
const SDL_Color TEXT_PLACEHOLDER_COLOR = {128, 128, 128, 0};

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 4096;

const int GUNDAM_LASER_WIDTH = 80;
const int GUNDAM_LASER_HIGHT = 682;
const double GUNDAM_LASER_DAMAGE = 560;
const double GUNDAM_LASER_UPGRADE = 320;

const int HEALTH_BAR_WIDTH = 256;
const int HEALTH_BAR_HEIGHT = 80;
const int HEALTH_BORDER_WIDTH = 277;
const int HEALTH_BORDER_HEIGHT = 87;

int Rand(int l, int r);

double dist(double x, double y, double u, double v);

double to_radian(double degree);
double to_degree(double radian);

double get_angle_from_step(double x, double y);

struct Texture {
    SDL_Texture *texture;
    int w, h;
};

#endif // INIT_H_INCLUDED
