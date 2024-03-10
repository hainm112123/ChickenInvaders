#include "init.h"

int Rand(int l, int r) {
    return l + (rand() % (r - l + 1));
}

//.............................Entity..............................................
Entity::Entity() {
    texture = {NULL, 0, 0};
}
Entity::Entity(EntityType _type, SDL_Rect _rect, Texture _texture): type(_type), rect(_rect), texture(_texture) {}

void Entity::setRect(int x, int y) {
    rect.x = x; rect.y = y;
}
void Entity::setRect(SDL_Rect _rect) {
    rect = _rect;
}
void Entity::setStep(int _step_x, int _step_y) {
    step_x = _step_x;
    step_y = _step_y;
}
void Entity::updateStep(int det_x, int det_y) {
    step_x += det_x;
    step_y += det_y;
}
void Entity::_move(bool isInsideScreen) {
    if (isInsideScreen && (rect.x + step_x < 0 || rect.x + step_x > SCREEN_WIDTH - rect.w || rect.y + step_y < 0 || rect.y + step_y > SCREEN_HEIGHT - rect.h)) return;
//    cout << step_x << " " << step_y << "\n";
    rect.x += step_x;
    rect.y += step_y;
}

bool Entity::isInsideScreen() {
    return (rect.x >= 0 && rect.x <= SCREEN_WIDTH && rect.y >= 0 && rect.y <= SCREEN_HEIGHT);
}

bool Entity::collisionWith(const Entity &entity) {
    int x = max(rect.x, entity.rect.x), y = max(rect.y, entity.rect.y),
        u = min(rect.x + rect.w, entity.rect.x + entity.rect.w), v = min(rect.y + rect.h, entity.rect.y + entity.rect.h);
    if (u - x > 3 && v - y > 3) return true;
    return false;
}

void Entity::render(SDL_Renderer *renderer, int arg) {
    if (type == LASER) {
        int n = 3, m = 4;
        (frame += 1) %= (n * m * FRAME_PER_PICTURE);
        int index = frame / 5;
        int i = index % m, j = index / m;
        int w = texture.w / m, h = texture.h / n;
        SDL_Rect src = {w*i + w/2 - GUNDAM_LASER_WIDTH/2, h*j, GUNDAM_LASER_WIDTH, GUNDAM_LASER_HIGHT};
        SDL_RenderCopy(renderer, texture.texture, &src, &rect);
    } else if (type == CHICKEN || type == CHICKEN_BOSS) {
        int n = textures.size();
        (frame += 1) %= (n * FRAME_PER_PICTURE_MORE);
        SDL_RenderCopy(renderer, textures[frame / FRAME_PER_PICTURE_MORE].texture, NULL, &rect);
    }
    else if (type == MENU || type == SHIELD || type == HEART) {
        SDL_RenderCopy(renderer, texture.texture, NULL, &rect);
    }
    else if (type == BACKGROUND) {
        SDL_Rect src = {0, texture.h - arg - SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT};
//            cout << src.y << " " << src.y + src.h << "\n";
        SDL_Rect dst = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, texture.texture, &src, &dst);
    }
    else if (type == ROCK) {
        int n = 7, m = 7;
        int cnt = 48;
        (frame += 1) %= (cnt * FRAME_PER_PICTURE);
        int index = frame / FRAME_PER_PICTURE;
        int w = texture.w / m, h = texture.h / n;
        int offsetX = 15, offsetY = 18;
        SDL_Rect src = {(index % m) * w + offsetX, (index / m) * h + offsetY, w - offsetX, h - offsetY};
        SDL_RenderCopy(renderer, texture.texture, &src, &rect);
    }
    else if (type == LEVEL_UP || type == EXPLOSION) {
        int n = 1;
        switch(type) {
            case LEVEL_UP:
                n = 25;
                break;
            case EXPLOSION:
                n = NUMBER_OF_EXPLOSION_PIC;
                break;
            default:
                break;
        }
        (frame += 1) %= (n * FRAME_PER_PICTURE);
        int ind = frame / FRAME_PER_PICTURE;
        int w = texture.w / n, h = texture.h;
        if (type != EXPLOSION) {
            rect.w = w; rect.h = h;
        }
        SDL_Rect src = {ind * w, 0, w, h};
        SDL_RenderCopy(renderer, texture.texture, &src, &rect);
    }
    else {
        rect.w = texture.w; rect.h = texture.h;
        SDL_RenderCopy(renderer, texture.texture, NULL, &rect);
    }
}

void Entity::setTexture(Texture _texture, bool isSetRect) {
    texture = _texture;
    if (isSetRect) {
        rect.w = texture.w;
        rect.h = texture.h;
    }
}

void Entity::setTextures(vector< Texture > _textures) {
    textures = _textures;
}

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
            painter->loadTexture("./graphics/phoenix/Phoenix_Chicken_ (4).png"),
            painter->loadTexture("./graphics/phoenix/Phoenix_Chicken_.png"),
            painter->loadTexture("./graphics/phoenix/Phoenix_Chicken_ (1).png"),
            painter->loadTexture("./graphics/phoenix/Phoenix_Chicken_ (2).png"),
            painter->loadTexture("./graphics/phoenix/Phoenix_Chicken_ (3).png"),
            painter->loadTexture("./graphics/phoenix/Phoenix_Chicken_ (2).png"),
            painter->loadTexture("./graphics/phoenix/Phoenix_Chicken_ (1).png"),
            painter->loadTexture("./graphics/phoenix/Phoenix_Chicken_.png"),
        },
        {
            painter->loadTexture("./graphics/boss1/Militarychicken (0).png"),
            painter->loadTexture("./graphics/boss1/Militarychicken (1).png"),
            painter->loadTexture("./graphics/boss1/Militarychicken (2).png"),
            painter->loadTexture("./graphics/boss1/Militarychicken (3).png"),
            painter->loadTexture("./graphics/boss1/Militarychicken (4).png"),
            painter->loadTexture("./graphics/boss1/Militarychicken (3).png"),
            painter->loadTexture("./graphics/boss1/Militarychicken (2).png"),
            painter->loadTexture("./graphics/boss1/Militarychicken (1).png"),
        },
        {
            painter->loadTexture("./graphics/boss2/Henperorsapprentice (0).png"),
            painter->loadTexture("./graphics/boss2/Henperorsapprentice (1).png"),
            painter->loadTexture("./graphics/boss2/Henperorsapprentice (2).png"),
            painter->loadTexture("./graphics/boss2/Henperorsapprentice (3).png"),
            painter->loadTexture("./graphics/boss2/Henperorsapprentice (4).png"),
            painter->loadTexture("./graphics/boss2/Henperorsapprentice (3).png"),
            painter->loadTexture("./graphics/boss2/Henperorsapprentice (2).png"),
            painter->loadTexture("./graphics/boss2/Henperorsapprentice (1).png"),
        },
        {
            painter->loadTexture("./graphics/boss3/CI4_InfiniChick (0).png"),
            painter->loadTexture("./graphics/boss3/CI4_InfiniChick (1).png"),
            painter->loadTexture("./graphics/boss3/CI4_InfiniChick (2).png"),
            painter->loadTexture("./graphics/boss3/CI4_InfiniChick (3).png"),
            painter->loadTexture("./graphics/boss3/CI4_InfiniChick (4).png"),
            painter->loadTexture("./graphics/boss3/CI4_InfiniChick (3).png"),
            painter->loadTexture("./graphics/boss3/CI4_InfiniChick (2).png"),
            painter->loadTexture("./graphics/boss3/CI4_InfiniChick (1).png"),
        }
    };
    gundamWeapons = {
        {
            painter->loadTexture("./graphics/blaster0.png"),
            painter->loadTexture("./graphics/blaster1.png"),
            painter->loadTexture("./graphics/blaster2.png"),
            painter->loadTexture("./graphics/blaster3.png"),
        },
        {
            painter->loadTexture("./graphics/boron0.png"),
            painter->loadTexture("./graphics/boron1.png"),
            painter->loadTexture("./graphics/boron2.png"),
            painter->loadTexture("./graphics/boron3.png"),
        },
        {
            painter->loadTexture("./graphics/neutron0.png"),
            painter->loadTexture("./graphics/neutron1.png"),
            painter->loadTexture("./graphics/neutron2.png"),
            painter->loadTexture("./graphics/neutron3.png"),
        },
    };
    eggs = {
        painter->loadTexture("./graphics/egg.png"),
        painter->loadTexture("./graphics/egg_boss.png"),
    };
    gundams = {
        painter->loadTexture("./graphics/player-red-1.png"),
        painter->loadTexture("./graphics/player-blue-1.png"),
        painter->loadTexture("./graphics/player-green-1.png"),
    };
    rocks = {
        painter->loadTexture("./graphics/asteroid1.png"),
        painter->loadTexture("./graphics/asteroid2.png"),
        painter->loadTexture("./graphics/asteroid3.png"),
    };

    laser = painter->loadTexture("./graphics/texture_laser.png");
    background = painter->loadTexture("./graphics/CI1Space_1.png");
    expolosion = painter->loadTexture("./graphics/explosion.png");

    shield = painter->loadTexture("./graphics/spr_shield.png");
    levelUp = painter->loadTexture("./graphics/level_up.png");
    newWeapons = {
        painter->loadTexture("./graphics/gift0.png"),
        painter->loadTexture("./graphics/gift1.png"),
        painter->loadTexture("./graphics/gift2.png"),
        painter->loadTexture("./graphics/gift_laser.png")
    };

    menu = painter->loadTexture("./graphics/menu.png");
    menu_settings = painter->loadTexture("./graphics/CI1Space.png");
    heart = painter->loadTexture("./graphics/heart.png");
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
        Mix_LoadWAV("./sound/blaster.wav"),
        Mix_LoadWAV("./sound/boron.wav"),
        Mix_LoadWAV("./sound/neutron.wav"),
    };
    chickens = {
        Mix_LoadWAV("./sound/ChickenHit.wav"),
        Mix_LoadWAV("./sound/ChickenHit2.wav"),
        Mix_LoadWAV("./sound/ChickenHit3.wav"),
    };
    explosions = {
        Mix_LoadWAV("./sound/exp.wav"),
        Mix_LoadWAV("./sound/exp_uco.wav"),
    };
    upgrade = Mix_LoadWAV("./sound/level_up.wav");
    music = Mix_LoadMUS("./sound/start.mp3");
    bulletRock = Mix_LoadWAV("./sound/whistle.wav");
    laser = Mix_LoadWAV("./sound/laser8.wav");
}

//.........................timer........................................
Timer::Timer(double _duration): duration(_duration) {}
void Timer::setDuration(double _duration) {
    duration = _duration;
}

void Timer::startCountdown() {
    active = true;
    st = CLOCK_NOW();
}

bool Timer::timeIsUp() {
    if (!active) return true;
    ed = CLOCK_NOW();
    ElapsedTime elapsed = ed - st;
    if (elapsed.count() > duration) {
        active = false;
        return true;
    }
    return false;
}
