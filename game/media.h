#ifndef MEDIA_H_INCLUDED
#define MEDIA_H_INCLUDED

#include <SDL_mixer.h>
#include <SDL_image.h>

#include "../common/common.h"

class Gallery {
    static Gallery *instance;
    SDL_Renderer *renderer;
public:
    static Gallery *Instance() {
        return instance;
    }
    static void initInstance(SDL_Renderer *_renderer) {
        instance = new Gallery(_renderer);
    }

    vector<vector<Texture>> gundamWeapons, chickens;
    vector<Texture> eggs, gundams, newWeapons, rocks, expolosions;
    Texture background, menu, menu_settings, menu_control;
    Texture laser, levelUp, shield, heart, level, rocket_mini, fry_chicken_mini;
    Texture bossHealthBar, bossHealthBorder;
    Texture fried_chicken, rocket, big_explosion;

    Gallery(SDL_Renderer *_renderer);
    ~Gallery();

    Texture loadTexture(string path);
    void loadGamePictures();
};

class Media {
    static Media *instance;
public:
    static Media *Instance() {
        if (instance == nullptr) instance = new Media();
        return instance;
    }

    vector<Mix_Chunk*> bullets, explosions, chickens;
    Mix_Chunk *upgrade, *bulletRock, *laser;
    Mix_Music *music;

    Media();
    ~Media();

    void loadMedia();
};

#endif // MEDIA_H_INCLUDED
