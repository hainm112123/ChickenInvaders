#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <SDL_ttf.h>
#include "../common/common.h"
#include "../common/entity.h"

class Text : public Entity {
    string text;
    SDL_Color color;

public:
    Text();
    ~Text();
    Text(const string &_text, const SDL_Color &_color);

    bool color_equal(SDL_Color other) {
        return color.r == other.r && color.g == other.g && color.b == other.b && color.a == other.a;
    }

    void setText(const string &_text);
    void setColor(const SDL_Color &_color);

    void renderText(TTF_Font *font, SDL_Renderer *renderer, bool init = false);
    void release();
};

#endif // TEXT_H_INCLUDED
