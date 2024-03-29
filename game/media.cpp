#include "media.h"

// ........................Galery...............................
Gallery *Gallery::instance = nullptr;

Gallery::Gallery(SDL_Renderer *_renderer): renderer(_renderer) {
    loadGamePictures();
}

Texture Gallery::loadTexture(string path) {
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    int w, h;
    if (loadedSurface == NULL) {
        cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << "\n";
    }
    else {
        Uint32 colorKey = SDL_MapRGB(loadedSurface->format, 170, 170, 170);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, colorKey);
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        w = loadedSurface->w; h = loadedSurface->h;
        if (newTexture == NULL) {
            cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << "\n";
        }
        SDL_FreeSurface(loadedSurface);
    }
    return {newTexture, w, h};
}

Gallery::~Gallery() {
    for (auto textures: gundamWeapons) for (Texture texture: textures) SDL_DestroyTexture(texture.texture);
    for (auto textures: chickens) for (Texture texture: textures) SDL_DestroyTexture(texture.texture);
    for (Texture texture: eggs) SDL_DestroyTexture(texture.texture);
    for (auto textures: gundams) for (Texture texture: textures) SDL_DestroyTexture(texture.texture);
    for (Texture texture: newWeapons) SDL_DestroyTexture(texture.texture);
    for (Texture texture: rocks) SDL_DestroyTexture(texture.texture);
    for (Texture texture: expolosions) SDL_DestroyTexture(texture.texture);
    for (Texture texture: fried_chickens) SDL_DestroyTexture(texture.texture);
    SDL_DestroyTexture(laser.texture);
    SDL_DestroyTexture(background.texture);
    SDL_DestroyTexture(levelUp.texture);
    SDL_DestroyTexture(menu.texture);
    SDL_DestroyTexture(shield.texture);
    SDL_DestroyTexture(menu_settings.texture);
    SDL_DestroyTexture(heart.texture);
    SDL_DestroyTexture(bossHealthBar.texture);
    SDL_DestroyTexture(bossHealthBorder.texture);
    SDL_DestroyTexture(menu_control.texture);
    SDL_DestroyTexture(level.texture);
    SDL_DestroyTexture(rocket_mini.texture);
    SDL_DestroyTexture(fry_chicken_mini.texture);
    SDL_DestroyTexture(rocket.texture);
    SDL_DestroyTexture(big_explosion.texture);
    SDL_DestroyTexture(pause_button.texture);
    SDL_DestroyTexture(home_button.texture);
    SDL_DestroyTexture(resume_button.texture);
    SDL_DestroyTexture(audio_muted_button.texture);
    SDL_DestroyTexture(audio_unmuted_button.texture);
    SDL_DestroyTexture(pause_menu.texture);
    SDL_DestroyTexture(teleport.texture);

    instance = nullptr;
}

void Gallery::loadGamePictures() {
    chickens = {
        {
            loadTexture("./assets/graphics/chicken/chicken (0).png"),
            loadTexture("./assets/graphics/chicken/chicken (1).png"),
            loadTexture("./assets/graphics/chicken/chicken (2).png"),
            loadTexture("./assets/graphics/chicken/chicken (3).png"),
            loadTexture("./assets/graphics/chicken/chicken (4).png"),
            loadTexture("./assets/graphics/chicken/chicken (3).png"),
            loadTexture("./assets/graphics/chicken/chicken (2).png"),
            loadTexture("./assets/graphics/chicken/chicken (1).png"),
        },
        {
            loadTexture("./assets/graphics/boss1/Militarychicken (0).png"),
            loadTexture("./assets/graphics/boss1/Militarychicken (1).png"),
            loadTexture("./assets/graphics/boss1/Militarychicken (2).png"),
            loadTexture("./assets/graphics/boss1/Militarychicken (3).png"),
            loadTexture("./assets/graphics/boss1/Militarychicken (4).png"),
            loadTexture("./assets/graphics/boss1/Militarychicken (3).png"),
            loadTexture("./assets/graphics/boss1/Militarychicken (2).png"),
            loadTexture("./assets/graphics/boss1/Militarychicken (1).png"),
        },
//        {
//            loadTexture("./assets/graphics/boss2/Henperorsapprentice (0).png"),
//            loadTexture("./assets/graphics/boss2/Henperorsapprentice (1).png"),
//            loadTexture("./assets/graphics/boss2/Henperorsapprentice (2).png"),
//            loadTexture("./assets/graphics/boss2/Henperorsapprentice (3).png"),
//            loadTexture("./assets/graphics/boss2/Henperorsapprentice (4).png"),
//            loadTexture("./assets/graphics/boss2/Henperorsapprentice (3).png"),
//            loadTexture("./assets/graphics/boss2/Henperorsapprentice (2).png"),
//            loadTexture("./assets/graphics/boss2/Henperorsapprentice (1).png"),
//        },
//        {
//            loadTexture("./assets/graphics/boss3/CI4_InfiniChick (0).png"),
//            loadTexture("./assets/graphics/boss3/CI4_InfiniChick (1).png"),
//            loadTexture("./assets/graphics/boss3/CI4_InfiniChick (2).png"),
//            loadTexture("./assets/graphics/boss3/CI4_InfiniChick (3).png"),
//            loadTexture("./assets/graphics/boss3/CI4_InfiniChick (4).png"),
//            loadTexture("./assets/graphics/boss3/CI4_InfiniChick (3).png"),
//            loadTexture("./assets/graphics/boss3/CI4_InfiniChick (2).png"),
//            loadTexture("./assets/graphics/boss3/CI4_InfiniChick (1).png"),
//        },
        {
            loadTexture("./assets/graphics/chicken/chicken (0).png"),
            loadTexture("./assets/graphics/chicken/chicken (1).png"),
            loadTexture("./assets/graphics/chicken/chicken (2).png"),
            loadTexture("./assets/graphics/chicken/chicken (3).png"),
            loadTexture("./assets/graphics/chicken/chicken (4).png"),
            loadTexture("./assets/graphics/chicken/chicken (3).png"),
            loadTexture("./assets/graphics/chicken/chicken (2).png"),
            loadTexture("./assets/graphics/chicken/chicken (1).png"),
        },
    };
    gundamWeapons = {
        {
            loadTexture("./assets/graphics/blaster0.png"),
            loadTexture("./assets/graphics/blaster1.png"),
            loadTexture("./assets/graphics/blaster2.png"),
            loadTexture("./assets/graphics/blaster3.png"),
        },
        {
            loadTexture("./assets/graphics/boron0.png"),
            loadTexture("./assets/graphics/boron1.png"),
            loadTexture("./assets/graphics/boron2.png"),
            loadTexture("./assets/graphics/boron3.png"),
        },
        {
            loadTexture("./assets/graphics/neutron0.png"),
            loadTexture("./assets/graphics/neutron1.png"),
            loadTexture("./assets/graphics/neutron2.png"),
            loadTexture("./assets/graphics/neutron3.png"),
        },
        {
            loadTexture("./assets/graphics/aim-laser-0.png"),
            loadTexture("./assets/graphics/aim-laser-1.png"),
            loadTexture("./assets/graphics/aim-laser-2.png"),
            loadTexture("./assets/graphics/aim-laser-3.png"),
        },
    };
    eggs = {
        loadTexture("./assets/graphics/egg.png"),
        loadTexture("./assets/graphics/egg_boss.png"),
    };
    gundams = {
//        loadTexture("./assets/graphics/player-red-1.png"),
//        loadTexture("./assets/graphics/player-blue-1.png"),
//        loadTexture("./assets/graphics/player-green-1.png"),
//        loadTexture("./assets/graphics/player-black.png"),
        {
            loadTexture("./assets/graphics/gundam/gundam_left_2_1.png"),
            loadTexture("./assets/graphics/gundam/gundam_left_2_2.png"),
            loadTexture("./assets/graphics/gundam/gundam_left_2_3.png"),
            loadTexture("./assets/graphics/gundam/gundam_left_2_2.png")
        },
        {
            loadTexture("./assets/graphics/gundam/gundam_left_1_1.png"),
            loadTexture("./assets/graphics/gundam/gundam_left_1_2.png"),
            loadTexture("./assets/graphics/gundam/gundam_left_1_3.png"),
            loadTexture("./assets/graphics/gundam/gundam_left_1_2.png")
        },
        {
            loadTexture("./assets/graphics/gundam/gundam_center_1.png"),
            loadTexture("./assets/graphics/gundam/gundam_center_2.png"),
            loadTexture("./assets/graphics/gundam/gundam_center_3.png"),
            loadTexture("./assets/graphics/gundam/gundam_center_2.png")
        },
        {
            loadTexture("./assets/graphics/gundam/gundam_right_1_1.png"),
            loadTexture("./assets/graphics/gundam/gundam_right_1_2.png"),
            loadTexture("./assets/graphics/gundam/gundam_right_1_3.png"),
            loadTexture("./assets/graphics/gundam/gundam_right_1_2.png")
        },
        {
            loadTexture("./assets/graphics/gundam/gundam_right_2_1.png"),
            loadTexture("./assets/graphics/gundam/gundam_right_2_2.png"),
            loadTexture("./assets/graphics/gundam/gundam_right_2_3.png"),
            loadTexture("./assets/graphics/gundam/gundam_right_2_2.png")
        }
    };
    rocks = {
        loadTexture("./assets/graphics/asteroid1.png"),
        loadTexture("./assets/graphics/asteroid2.png"),
        loadTexture("./assets/graphics/asteroid3.png"),
    };
    expolosions = {
        loadTexture("./assets/graphics/smoke.png"),
        loadTexture("./assets/graphics/explosion.png"),
    };

    laser = loadTexture("./assets/graphics/texture_laser.png");
    background = loadTexture("./assets/graphics/CI1Space_1.png");

    shield = loadTexture("./assets/graphics/shield_1.png");
    levelUp = loadTexture("./assets/graphics/level_up.png");
    newWeapons = {
        loadTexture("./assets/graphics/gift0.png"),
        loadTexture("./assets/graphics/gift1.png"),
        loadTexture("./assets/graphics/gift2.png"),
        loadTexture("./assets/graphics/GIFTUtensilPoker.png"),
        loadTexture("./assets/graphics/gift_laser.png"),
    };

    menu = loadTexture("./assets/graphics/menu.png");
    menu_settings = loadTexture("./assets/graphics/CI1Space.png");
    menu_control = loadTexture("./assets/graphics/control.png");

    bossHealthBar = loadTexture("./assets/graphics/HealthBar.png");
    bossHealthBorder = loadTexture("./assets/graphics/HealthBorder.png");

    heart = loadTexture("./assets/graphics/heart.png");
    level = loadTexture("./assets/graphics/thunder.png");
    rocket_mini = loadTexture("./assets/graphics/rocket_mini.png");
    fry_chicken_mini = loadTexture("./assets/graphics/fried-chicken-mini.png");

    fried_chickens = {
        loadTexture("./assets/graphics/fried_chicken/fried-chicken.png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (8).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (1).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (9).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (2).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (10).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (3).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (11).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (4).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (12).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (5).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (13).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (6).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (14).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (7).png"),
        loadTexture("./assets/graphics/fried_chicken/fried-chicken (15).png"),
    };
    rocket = loadTexture("./assets/graphics/rocket.png");
    big_explosion = loadTexture("./assets/graphics/big_exp-transformed.png");

    pause_button = loadTexture("./assets/graphics/pause_menu/pause.png");
    home_button = loadTexture("./assets/graphics/pause_menu/home_button.png");
    resume_button = loadTexture("./assets/graphics/pause_menu/resume_button.png");
    audio_muted_button = loadTexture("./assets/graphics/pause_menu/audio_muted.png");
    audio_unmuted_button = loadTexture("./assets/graphics/pause_menu/audio_unmuted.png");
    pause_menu = loadTexture("./assets/graphics/pause_menu/pause_menu.png");

    teleport = loadTexture("./assets/graphics/tele.png");
}

//.............................Media............................................
Media *Media::instance = nullptr;
Media::Media() {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        cout << "Failed to open audio\n";
        return;
    }
    loadMedia();
}

Media::~Media() {
    Mix_FreeMusic(start);
    Mix_FreeMusic(battle);
    Mix_FreeMusic(ending);

    for (Mix_Chunk *bullet: bullets) Mix_FreeChunk(bullet);
    for (Mix_Chunk *chicken: chickens) Mix_FreeChunk(chicken);
    for (Mix_Chunk *explosion: explosions) Mix_FreeChunk(explosion);

    Mix_FreeChunk(upgrade);
    Mix_FreeChunk(laser);
    Mix_FreeChunk(bulletRock);
    Mix_FreeChunk(rocket);
    Mix_FreeChunk(bite);
    Mix_FreeChunk(tele);

    Mix_CloseAudio();

    instance = nullptr;
}

void Media::loadMedia() {
    bullets = {
        Mix_LoadWAV("./assets/sound/blaster.wav"),
        Mix_LoadWAV("./assets/sound/boron.wav"),
        Mix_LoadWAV("./assets/sound/neutron.wav"),
        Mix_LoadWAV("./assets/sound/laser1.wav"),
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
    bulletRock = Mix_LoadWAV("./assets/sound/whistle.wav");
    laser = Mix_LoadWAV("./assets/sound/laser8.wav");
    rocket = Mix_LoadWAV("./assets/sound/rocket.wav");
    bite = Mix_LoadWAV("./assets/sound/bite.mp3");
    tele = Mix_LoadWAV("./assets/sound/tele.wav");

    start = Mix_LoadMUS("./assets/sound/Zoltraak.mp3");
    battle = Mix_LoadMUS("./assets/sound/battle.mp3");
    ending = Mix_LoadMUS("./assets/sound/Fear Brought Me This Far.mp3");
}
