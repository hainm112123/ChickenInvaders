#include "text.h"

Text::Text() {
    type = TEXT;
}
Text::Text(const string &_text, const SDL_Color &_color): text(_text), color(_color) {
    type = TEXT;
}
Text::~Text() {
    release();
}

void Text::setText(const string &_text) {
    text = _text;
}
void Text::setColor(const SDL_Color &_color) {
    color = _color;
}

void Text::renderText(TTF_Font *font, SDL_Renderer *renderer, bool init) {
    release();
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface != NULL) {
        texture = {SDL_CreateTextureFromSurface(renderer, surface), surface->w, surface->h};
        if (texture.texture != NULL) {
            rect.w = texture.w;
            rect.h = texture.h;
        }
        SDL_FreeSurface(surface);
    }
    if (!init) render(renderer);
}

void Text::release() {
    if (texture.texture != NULL) {
        SDL_DestroyTexture(texture.texture);
        texture = {NULL, 0, 0};
    }
}
