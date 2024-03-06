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
    if (type == MENU) {
        SDL_RenderCopy(renderer, texture.texture, NULL, &rect);
    }
    if (type == BACKGROUND) {
//        cout << arg << "\n";
        int n = (SCREEN_WIDTH - 1) / texture.w + 1, m = (SCREEN_HEIGHT - arg - 1) / texture.h + 1;
        for (int i = 0; i < n; ++ i) {
            SDL_Rect src = {0, texture.h - arg, texture.w, arg};
            SDL_Rect dst = {texture.w * i, 0, texture.w, arg};
            SDL_RenderCopy(renderer, texture.texture, &src, &dst);
        }
        rect.w = texture.w; rect.h = texture.h;
        for (int i = 0; i < n; ++ i) for (int j = 0; j < m; ++ j) {
            rect.x = rect.w * i; rect.y = rect.h * j + arg;
//            cout << rect.x << " " << rect.y << "\n";
            SDL_RenderCopy(renderer, texture.texture, NULL, &rect);
        }
    }
    else if (type == ROCK) {
        int n = 3, m = 4;
        (frame += 1) %= (n * m * FRAME_PER_PICTURE);
        int index = frame / FRAME_PER_PICTURE;
        int w = texture.w / m, h = texture.h / n;
        SDL_Rect src = {(index % m) * w, (index / m) * h, w, h};
        SDL_RenderCopy(renderer, texture.texture, &src, &rect);
    }
    else if (type == CHICKEN || type == CHICKEN_BOSS || type == LEVEL_UP || type == EXPLOSION) {
        int n = 1;
        switch(type) {
            case CHICKEN:
                n = 18;
                break;
            case CHICKEN_BOSS:
                n = 10;
                break;
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

void Entity::setTexture(Texture _texture) {
    texture = _texture;
}

// ........................Galery...............................
Gallery::Gallery(Painter *_painter): painter(_painter) {
    loadGamePictures();
}

Gallery::~Gallery() {
    for (auto textures: gundamWeapons) for (Texture texture: textures) SDL_DestroyTexture(texture.texture);
    for (Texture texture: chickens) SDL_DestroyTexture(texture.texture);
    for (Texture texture: eggs) SDL_DestroyTexture(texture.texture);
}

void Gallery::loadGamePictures() {
    chickens = {
        painter->loadTexture("./graphics/chicken.png"),
        painter->loadTexture("./graphics/boss.png"),
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

    background = painter->loadTexture("./graphics/background-blue.png");
    rock = painter->loadTexture("./graphics/rock_round.png");
    laser = painter->loadTexture("./graphics/texture_laser.png");

    levelUp = painter->loadTexture("./graphics/level_up.png");
    newWeapons = {
        painter->loadTexture("./graphics/gift0.png"),
        painter->loadTexture("./graphics/gift1.png"),
        painter->loadTexture("./graphics/gift2.png")
    };

    expolosion = painter->loadTexture("./graphics/explosion.png");
    rock = painter->loadTexture("./graphics/rock_round.png");

    menu = painter->loadTexture("./graphics/menu.png");
}
