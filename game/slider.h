#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include "../common/common.h"

const int BAR_WIDTH = 300;
const int BAR_HEIGHT = 4;
const int KNOB_WIDTH = 6;
const int KNOB_WIDTH_ACTIVE = 10;
const int KNOB_HEIGHT = 16;
const int KNOB_HEIGHT_ACTIVE = 24;

class Slider {
    SDL_Rect bar, bar_active, knob, knob_active;
    bool is_active = false;
    double value;

public:
    Slider();
    Slider(int c_x, int c_y);
    void init(int c_x, int c_y);
    void handleEvent(const SDL_Event e);
    void render(SDL_Renderer *renderer);

    double Value() const {
        return value;
    }
};

#endif // SLIDER_H_INCLUDED
