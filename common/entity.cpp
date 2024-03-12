#include "entity.h"

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
