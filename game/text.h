#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "../init.h"

class Text : public Entity {
    string text;
    SDL_Color color;

public:
    Text();
    ~Text();
    Text(const string &_text, const SDL_Color &_color);

    void setText(const string &_text);
    void setColor(const SDL_Color &_color);

    void renderText(TTF_Font *font, SDL_Renderer *renderer, bool init = false);
    void release();
};

#endif // TEXT_H_INCLUDED
