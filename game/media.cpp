#include "media.h"

// ........................Galery...............................
Gallery::Gallery(Painter *_painter): painter(_painter) {
    loadGamePictures();
}

Gallery::~Gallery() {
    for (auto textures: gundamWeapons) for (Texture texture: textures) SDL_DestroyTexture(texture.texture);
    for (auto textures: chickens) for (Texture texture: textures) SDL_DestroyTexture(texture.texture);
    for (Texture texture: eggs) SDL_DestroyTexture(texture.texture);
    for (Texture texture: gundams) SDL_DestroyTexture(texture.texture);
    for (Texture texture: newWeapons) SDL_DestroyTexture(texture.texture);
    for (Texture texture: rocks) SDL_DestroyTexture(texture.texture);
    SDL_DestroyTexture(laser.texture);
    SDL_DestroyTexture(background.texture);
    SDL_DestroyTexture(levelUp.texture);
    SDL_DestroyTexture(expolosion.texture);
    SDL_DestroyTexture(menu.texture);
    SDL_DestroyTexture(shield.texture);
    SDL_DestroyTexture(menu_settings.texture);
    SDL_DestroyTexture(heart.texture);
}

void Gallery::loadGamePictures() {
    chickens = {
        {
            painter->loadTexture("./assets/graphics/phoenix/Phoenix_Chicken_ (4).png"),
            painter->loadTexture("./assets/graphics/phoenix/Phoenix_Chicken_.png"),
            painter->loadTexture("./assets/graphics/phoenix/Phoenix_Chicken_ (1).png"),
            painter->loadTexture("./assets/graphics/phoenix/Phoenix_Chicken_ (2).png"),
            painter->loadTexture("./assets/graphics/phoenix/Phoenix_Chicken_ (3).png"),
            painter->loadTexture("./assets/graphics/phoenix/Phoenix_Chicken_ (2).png"),
            painter->loadTexture("./assets/graphics/phoenix/Phoenix_Chicken_ (1).png"),
            painter->loadTexture("./assets/graphics/phoenix/Phoenix_Chicken_.png"),
        },
        {
            painter->loadTexture("./assets/graphics/boss1/Militarychicken (0).png"),
            painter->loadTexture("./assets/graphics/boss1/Militarychicken (1).png"),
            painter->loadTexture("./assets/graphics/boss1/Militarychicken (2).png"),
            painter->loadTexture("./assets/graphics/boss1/Militarychicken (3).png"),
            painter->loadTexture("./assets/graphics/boss1/Militarychicken (4).png"),
            painter->loadTexture("./assets/graphics/boss1/Militarychicken (3).png"),
            painter->loadTexture("./assets/graphics/boss1/Militarychicken (2).png"),
            painter->loadTexture("./assets/graphics/boss1/Militarychicken (1).png"),
        },
        {
            painter->loadTexture("./assets/graphics/boss2/Henperorsapprentice (0).png"),
            painter->loadTexture("./assets/graphics/boss2/Henperorsapprentice (1).png"),
            painter->loadTexture("./assets/graphics/boss2/Henperorsapprentice (2).png"),
            painter->loadTexture("./assets/graphics/boss2/Henperorsapprentice (3).png"),
            painter->loadTexture("./assets/graphics/boss2/Henperorsapprentice (4).png"),
            painter->loadTexture("./assets/graphics/boss2/Henperorsapprentice (3).png"),
            painter->loadTexture("./assets/graphics/boss2/Henperorsapprentice (2).png"),
            painter->loadTexture("./assets/graphics/boss2/Henperorsapprentice (1).png"),
        },
        {
            painter->loadTexture("./assets/graphics/boss3/CI4_InfiniChick (0).png"),
            painter->loadTexture("./assets/graphics/boss3/CI4_InfiniChick (1).png"),
            painter->loadTexture("./assets/graphics/boss3/CI4_InfiniChick (2).png"),
            painter->loadTexture("./assets/graphics/boss3/CI4_InfiniChick (3).png"),
            painter->loadTexture("./assets/graphics/boss3/CI4_InfiniChick (4).png"),
            painter->loadTexture("./assets/graphics/boss3/CI4_InfiniChick (3).png"),
            painter->loadTexture("./assets/graphics/boss3/CI4_InfiniChick (2).png"),
            painter->loadTexture("./assets/graphics/boss3/CI4_InfiniChick (1).png"),
        }
    };
    gundamWeapons = {
        {
            painter->loadTexture("./assets/graphics/blaster0.png"),
            painter->loadTexture("./assets/graphics/blaster1.png"),
            painter->loadTexture("./assets/graphics/blaster2.png"),
            painter->loadTexture("./assets/graphics/blaster3.png"),
        },
        {
            painter->loadTexture("./assets/graphics/boron0.png"),
            painter->loadTexture("./assets/graphics/boron1.png"),
            painter->loadTexture("./assets/graphics/boron2.png"),
            painter->loadTexture("./assets/graphics/boron3.png"),
        },
        {
            painter->loadTexture("./assets/graphics/neutron0.png"),
            painter->loadTexture("./assets/graphics/neutron1.png"),
            painter->loadTexture("./assets/graphics/neutron2.png"),
            painter->loadTexture("./assets/graphics/neutron3.png"),
        },
    };
    eggs = {
        painter->loadTexture("./assets/graphics/egg.png"),
        painter->loadTexture("./assets/graphics/egg_boss.png"),
    };
    gundams = {
        painter->loadTexture("./assets/graphics/player-red-1.png"),
        painter->loadTexture("./assets/graphics/player-blue-1.png"),
        painter->loadTexture("./assets/graphics/player-green-1.png"),
    };
    rocks = {
        painter->loadTexture("./assets/graphics/asteroid1.png"),
        painter->loadTexture("./assets/graphics/asteroid2.png"),
        painter->loadTexture("./assets/graphics/asteroid3.png"),
    };

    laser = painter->loadTexture("./assets/graphics/texture_laser.png");
    background = painter->loadTexture("./assets/graphics/CI1Space_1.png");
    expolosion = painter->loadTexture("./assets/graphics/explosion.png");

    shield = painter->loadTexture("./assets/graphics/spr_shield.png");
    levelUp = painter->loadTexture("./assets/graphics/level_up.png");
    newWeapons = {
        painter->loadTexture("./assets/graphics/gift0.png"),
        painter->loadTexture("./assets/graphics/gift1.png"),
        painter->loadTexture("./assets/graphics/gift2.png"),
        painter->loadTexture("./assets/graphics/gift_laser.png")
    };

    menu = painter->loadTexture("./assets/graphics/menu.png");
    menu_settings = painter->loadTexture("./assets/graphics/CI1Space.png");
    heart = painter->loadTexture("./assets/graphics/heart.png");
}

//.............................Media............................................
Media::Media() {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        cout << "Failed to open audio\n";
        return;
    }
    loadMedia();
}

Media::~Media() {
    for (Mix_Chunk *bullet: bullets) Mix_FreeChunk(bullet);
    for (Mix_Chunk *chicken: chickens) Mix_FreeChunk(chicken);
    for (Mix_Chunk *explosion: explosions) Mix_FreeChunk(explosion);
    Mix_FreeMusic(music);
    Mix_FreeChunk(upgrade);
    Mix_FreeChunk(laser);
    Mix_FreeChunk(bulletRock);
    Mix_CloseAudio();
}

void Media::loadMedia() {
    bullets = {
        Mix_LoadWAV("./assets/sound/blaster.wav"),
        Mix_LoadWAV("./assets/sound/boron.wav"),
        Mix_LoadWAV("./assets/sound/neutron.wav"),
    };
    chickens = {
        Mix_LoadWAV("./assets/sound/ChickenHit.wav"),
        Mix_LoadWAV("./assets/sound/ChickenHit2.wav"),
        Mix_LoadWAV("./assets/sound/ChickenHit3.wav"),
    };
    explosions = {
        Mix_LoadWAV("./assets/sound/exp.wav"),
        Mix_LoadWAV("./assets/sound/exp_uco.wav"),
    };
    upgrade = Mix_LoadWAV("./assets/sound/level_up.wav");
    music = Mix_LoadMUS("./assets/sound/start.mp3");
    bulletRock = Mix_LoadWAV("./assets/sound/whistle.wav");
    laser = Mix_LoadWAV("./assets/sound/laser8.wav");
}
