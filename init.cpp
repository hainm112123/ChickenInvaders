#include "init.h"

//.............................Entity..............................................
Entity::Entity() {
    texture = {NULL, 0, 0};
}
Entity::Entity(EntityType _type, SDL_Rect _rect): type(_type), rect(_rect) {
    texture = {NULL, 0, 0};
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

bool Entity::collisionWith(const Entity &entity) {
    int x = max(rect.x, entity.rect.x), y = max(rect.y, entity.rect.y),
        u = min(rect.x + rect.w, entity.rect.x + entity.rect.w), v = min(rect.y + rect.h, entity.rect.y + entity.rect.h);
    if (u - x > 3 && v - y > 3) return true;
    return false;
}

void Entity::render(SDL_Renderer *renderer) {
    if (type == BACKGROUND) {
        int n = (SCREEN_WIDTH - 1) / texture.w + 1, m = (SCREEN_HEIGHT - 1) / texture.h + 1;
        rect.w = texture.w; rect.h = texture.h;
        for (int i = 0; i < n; ++ i) for (int j = 0; j < m; ++ j) {
            rect.x = rect.w * i; rect.y = rect.h * j;
            SDL_RenderCopy(renderer, texture.texture, NULL, &rect);
        }
    }
    if (type == GUNDAM || type == BULLET) {
        rect.w = texture.w; rect.h = texture.h;
        SDL_RenderCopy(renderer, texture.texture, NULL, &rect);
    }
    if (type == CHICKEN || type == CHICKEN_BOSS) {
        int n = (type == CHICKEN) ? 18 : 10;
        (frame += 1) %= (n * 5);
        int ind = frame / 5;
        int w = texture.w / n, h = texture.h;
        rect.w = w; rect.h = h;
        SDL_Rect src = {ind * w, 0, w, h};
        SDL_RenderCopy(renderer, texture.texture, &src, &rect);
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
    for (Texture texture: chickens) SDL_DestroyTexture(texture.texture);
    for (Texture texture: blasters) SDL_DestroyTexture(texture.texture);
    for (Texture texture: borons) SDL_DestroyTexture(texture.texture);
    for (Texture texture: neutrons) SDL_DestroyTexture(texture.texture);
    for (Texture texture: eggs) SDL_DestroyTexture(texture.texture);
}

void Gallery::loadGamePictures() {
    chickens = {
        painter->loadTexture("./graphics/chicken.png"),
        painter->loadTexture("./graphics/boss.png"),
    };
    blasters = {
        painter->loadTexture("./graphics/blaster0.png"),
        painter->loadTexture("./graphics/blaster1.png"),
        painter->loadTexture("./graphics/blaster2.png"),
        painter->loadTexture("./graphics/blaster3.png"),
    };
    borons = {
        painter->loadTexture("./graphics/boron0.png"),
        painter->loadTexture("./graphics/boron1.png"),
        painter->loadTexture("./graphics/boron2.png"),
        painter->loadTexture("./graphics/boron3.png"),
    };
    neutrons = {
        painter->loadTexture("./graphics/neutron0.png"),
        painter->loadTexture("./graphics/neutron1.png"),
        painter->loadTexture("./graphics/neutron2.png"),
        painter->loadTexture("./graphics/neutron3.png"),
    };
    eggs = {
        painter->loadTexture("./graphics/egg.png"),
        painter->loadTexture("./graphics/egg_boss.png"),
    };
    background = painter->loadTexture("./graphics/background-blue.png");
    gundam = painter->loadTexture("./graphics/player-blue-1.png");
    rock = painter->loadTexture("./graphics/rock_round.png");
//    laser = painter->loadTexture("./graphics/texture_laser.tif");
}
