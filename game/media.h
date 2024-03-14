#ifndef MEDIA_H_INCLUDED
#define MEDIA_H_INCLUDED

#include <SDL_mixer.h>

#include "../common/common.h"
#include "../Painter.h"

class Gallery {
    Painter *painter;
public:
    vector<vector<Texture>> gundamWeapons, chickens;
    vector<Texture> eggs, gundams, newWeapons, rocks, expolosions;
    Texture background, menu, menu_settings, menu_control;
    Texture laser, levelUp, shield, heart;
    Texture bossHealthBar, bossHealthBorder;

    Gallery(Painter *_painter);
    ~Gallery();

    void loadGamePictures();
};

class Media {
public:
    vector<Mix_Chunk*> bullets, explosions, chickens;
    Mix_Chunk *upgrade, *bulletRock, *laser;
    Mix_Music *music;

    Media();
    ~Media();

    void loadMedia();
};

#endif // MEDIA_H_INCLUDED
