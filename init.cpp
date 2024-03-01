#include "init.h"


Entity::Entity() {
    rect = {0, 0, 0, 0};
    texture = NULL;
}
Entity::Entity(SDL_Rect _rect): rect(_rect) {
    texture = NULL;
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
    rect.x += step_x;
    rect.y += step_y;
}

bool Entity::collisionWith(const Entity &entity) {
    int x = max(rect.x, entity.rect.x), y = max(rect.y, entity.rect.y),
        u = min(rect.x + rect.w, entity.rect.x + entity.rect.w), v = min(rect.y + rect.h, entity.rect.y + entity.rect.h);
    if (u - x > 1 && v - y > 1) return true;
    return false;
}

void Entity::render(SDL_Renderer *renderer) {
    SDL_RenderFillRect(renderer, &rect);
//    SDL_RenderPresent(renderer);
}
