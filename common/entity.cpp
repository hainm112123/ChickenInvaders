#include "entity.h"
#include "time.h"

//.............................Entity..............................................
Entity::Entity() {
    texture = {NULL, 0, 0};
}
Entity::Entity(EntityType _type, SDL_Rect _rect, Texture _texture): type(_type), rect(_rect), texture(_texture) {
    x = rect.x; y = rect.y;
}
Entity::~Entity(){}

void Entity::setRect(int _x, int _y) {
    x = _x; y = _y;
    rect.x = x; rect.y = y;
}
void Entity::setRect(SDL_Rect _rect) {
    rect = _rect;
}

void Entity::setPosition(double _x, double _y) {
    x = _x; y = _y;
    rect.x = x; rect.y = y;
//    cout << x << " " << y << " " << rect.x << " " << rect.y << "\n";
}

void Entity::setStep(double _step_x, double _step_y) {
    step_x = _step_x;
    step_y = _step_y;
}
void Entity::updateStep(double det_x, double det_y) {
//    cerr << det_x << " " << det_y << "\n";
    step_x += det_x;
    step_y += det_y;
}

void Entity::_move(bool isInsideScreen) {
    double det_x = step_x * TimeManager::Instance()->getElapsedTime();
    double det_y = step_y * TimeManager::Instance()->getElapsedTime();
    x += det_x; y += det_y;
    if (isInsideScreen && (x < 0 || x > SCREEN_WIDTH - rect.w || y < 0 || y > SCREEN_HEIGHT - rect.h)) {
        x -= det_x; y -= det_y;
        return;
    }
    rect.x = x;
    rect.y = y;
}

bool Entity::isInsideScreen() {
    return (rect.x >= 0 && rect.x <= SCREEN_WIDTH && rect.y >= 0 && rect.y <= SCREEN_HEIGHT);
}

bool Entity::collisionWith(const Entity *entity) {
    int x = max(rect.x, (entity->rect).x), y = max(rect.y, (entity->rect).y),
        u = min(rect.x + rect.w, (entity->rect).x + (entity->rect).w), v = min(rect.y + rect.h, (entity->rect).y + (entity->rect).h);
    if (u - x > 3 && v - y > 3) return true;
    return false;
}
bool Entity::collisionWith(const Entity &entity) {
    return collisionWith(&entity);
}

void Entity::render(SDL_Renderer *renderer, int arg) {
    const double elapsed = TimeManager::Instance()->getElapsedTime();
    if (type == BOSS_HEALTH_BAR) {
        SDL_Rect src = {0, 0, arg, HEALTH_BAR_HEIGHT};
        SDL_Rect dst = rect;
        dst.w = arg;
        SDL_RenderCopy(renderer, texture.texture, &src, &dst);
    }
    else if (type == LASER) {
        int n = 3, m = 4;
        currentTime += elapsed;
        if (currentTime >= SECOND_PER_PICTURE_FASTER * m * n) currentTime -= SECOND_PER_PICTURE_FASTER * m * n;
        int index = int(currentTime / SECOND_PER_PICTURE_FASTER);
        int i = index % m, j = index / m;
        int w = texture.w / m, h = texture.h / n;
        SDL_Rect src = {w*i + w/2 - GUNDAM_LASER_WIDTH/2, h*j, GUNDAM_LASER_WIDTH, GUNDAM_LASER_HIGHT};
        SDL_RenderCopy(renderer, texture.texture, &src, &rect);
    } else if (type == CHICKEN) {
        int n = textures.size();
        currentTime += elapsed;
        double per_pic = SECOND_PER_PICTURE;
        if (currentTime >= per_pic * n) currentTime -= per_pic * n;
        SDL_RenderCopy(renderer, textures[int(currentTime / per_pic)].texture, NULL, &rect);
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
        currentTime += elapsed;
        if (currentTime >= SECOND_PER_PICTURE_FASTER * cnt) currentTime -= SECOND_PER_PICTURE_FASTER * cnt;
        int index = int(currentTime / SECOND_PER_PICTURE_FASTER);
        int w = texture.w / m, h = texture.h / n;
        int offsetX = 15, offsetY = 18;
        SDL_Rect src = {(index % m) * w + offsetX, (index / m) * h + offsetY, w - offsetX, h - offsetY};
        SDL_RenderCopy(renderer, texture.texture, &src, &rect);
    }
    else if (type == LEVEL_UP || type == EXPLOSION || type == BIG_EXPLOSION) {
        int n = 1;
        double perPic = SECOND_PER_PICTURE;
        switch(type) {
            case LEVEL_UP:
                n = 25;
                break;
            case EXPLOSION:
                n = NUMBER_OF_EXPLOSION_PIC;
                perPic = SECOND_PER_PICTURE;
                break;
            case BIG_EXPLOSION:
                n = NUMBER_OF_BIG_EXPLOSION_PIC;
                perPic = SECOND_PER_PICTURE_LONGER;
                break;
            default:
                break;
        }
        currentTime += elapsed;
        if (currentTime >= perPic * n) {
            if (type == EXPLOSION || type == BIG_EXPLOSION) return;
            currentTime -= perPic * n;
        }
        int ind = int(currentTime / perPic);
//        cout << ind << "\n";
        int w = texture.w / n, h = texture.h;
        if (type == LEVEL_UP) {
            rect.w = w; rect.h = h;
        }
        SDL_Rect src = {ind * w, 0, w, h};
        SDL_RenderCopy(renderer, texture.texture, &src, &rect);
    }
    else if (type == BULLET) {
        rect.w = texture.w; rect.h = texture.h;
        if (step_x != 0) {
            double deg = get_angle_from_step(step_x, step_y);
            SDL_RenderCopyEx(renderer, texture.texture, NULL, &rect, deg, NULL, SDL_FLIP_NONE);
        }
        else {
            SDL_RenderCopy(renderer, texture.texture, NULL, &rect);
        }
    }
    else if (type == ROCKET) {
        int n = 3;
        currentTime += elapsed;
        if (currentTime >= n * SECOND_PER_PICTURE) currentTime -= n * SECOND_PER_PICTURE;
        int index = int(currentTime / SECOND_PER_PICTURE);
        SDL_Rect src = {0, (texture.h/n) * index, texture.w, texture.h/n};
        double deg = get_angle_from_step(step_x, step_y);
//        cout << rect.x << " " << rect.y << " " << rect.w << " " << rect.h << "\n";
//        cout << src.x << " " << src.y << " " << src.w << " " << src.h << "\n";
//        SDL_RenderCopy(renderer, texture.texture, &src, &rect);
        SDL_RenderCopyEx(renderer, texture.texture, &src, &rect, deg, NULL, SDL_FLIP_NONE);
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

void Entity::resetTime() {
    currentTime = 0;
}
