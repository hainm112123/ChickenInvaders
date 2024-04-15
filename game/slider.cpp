#include "slider.h"

Slider::Slider() {}

Slider::Slider(int c_x, int c_y) {
    init(c_x, c_y);
}

void Slider::init(int c_x, int c_y) {
    is_active = false;
    value = 100;
    bar_active = bar = {c_x - BAR_WIDTH/2, c_y - BAR_HEIGHT/2, BAR_WIDTH, BAR_HEIGHT};
    knob = {bar_active.x + bar_active.w - KNOB_WIDTH, c_y - KNOB_HEIGHT/2, KNOB_WIDTH, KNOB_HEIGHT};
}

void Slider::render(SDL_Renderer *renderer) {
    setColor(renderer, GRAY_COLOR);
    SDL_RenderFillRect(renderer, &bar);
    setColor(renderer, WHITE_COLOR);
    knob_active = {knob.x + knob.w/2 - KNOB_WIDTH_ACTIVE/2, knob.y + knob.h/2 - KNOB_HEIGHT_ACTIVE/2, KNOB_WIDTH_ACTIVE, KNOB_HEIGHT_ACTIVE};
    SDL_RenderFillRect(renderer, &bar_active);
    if (is_active) {
        SDL_RenderFillRect(renderer, &knob_active);
    }
    else {
        SDL_RenderFillRect(renderer, &knob);
    }
}

void Slider::handleEvent(const SDL_Event e) {
    if (e.type == SDL_MOUSEMOTION) {
        SDL_Point mouse = {e.motion.x, e.motion.y};
        SDL_Rect rect = {bar.x - 10, bar.y - 25, bar.w + 30, bar.h + 50};
        if (SDL_PointInRect(&mouse, &rect) && (e.motion.state & SDL_BUTTON_LMASK)) {
            is_active = true;
            value = double(max(min(mouse.x - bar.x, bar.w), 0)) / bar.w;
            bar_active.w = max(min(mouse.x - bar_active.x, bar.w), 0);
            knob.x = mouse.x - knob.w/2;
            knob.x = max(knob.x, bar.x);
            knob.x = min(knob.x, bar.x + bar.w - knob.w);
        }
        else {
            is_active = false;
        }
    }
}
