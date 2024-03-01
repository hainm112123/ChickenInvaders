#include <iostream>
#include <cmath>

#include <SDL.h>
#include <SDL_image.h>
#include "Painter.h"

using namespace std;

Painter::Painter(SDL_Window *window, SDL_Renderer *_renderer): renderer(_renderer) {
    int width, height;
    SDL_RenderGetLogicalSize(renderer, &width, &height);
    if (width == 0 && height == 0) {
        SDL_GetWindowSize(window, &width, &height);
    }
    setPosition(width / 2, height / 2);
    setAngle(0);
    setColor(WHITE_COLOR);
    clearWithBgColor(BLACK_COLOR);
}

void Painter::setPosition(double x, double y) {
    this->x = x;
    this->y = y;
}

void Painter::setAngle(double angle) {
    this->angle = angle - floor(angle / 360) * 360;
}

void Painter::setColor(SDL_Color color) {
    this->color = color;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0);
}

void Painter::clearWithBgColor(SDL_Color bgColor) {
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0);
}

void Painter::moveForward(double length) {
    double pre_x = x, pre_y = y;
    jumpForward(length);
    SDL_RenderDrawLine(renderer, pre_x, pre_y, x, y);
}

void Painter::jumpForward(double length) {
    double rad = (angle / 180) * M_PI;
    x += cos(rad) * length;
    y -= sin(rad) * length;
}

void Painter::createSquare(double _size) {
    for (int i = 0; i < 4; ++ i) {
        moveForward(_size);
        turnLeft(90);
    }
}

void Painter::createCircle(double radius) {
    double rad = (angle / 180) * M_PI;
    int centerX = x + cos(rad) * radius;
    int centerY = y - sin(rad) * radius;
    int dx = radius, dy = 0, err = 0;
    while (dx >= dy) {
        SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
        SDL_RenderDrawPoint(renderer, centerX + dy, centerY + dx);
        SDL_RenderDrawPoint(renderer, centerX - dy, centerY + dx);
        SDL_RenderDrawPoint(renderer, centerX - dx, centerY + dy);
        SDL_RenderDrawPoint(renderer, centerX - dx, centerY - dy);
        SDL_RenderDrawPoint(renderer, centerX - dy, centerY - dx);
        SDL_RenderDrawPoint(renderer, centerX + dy, centerY - dx);
        SDL_RenderDrawPoint(renderer, centerX + dx, centerY - dy);

        if (err <= 0) {
            dy ++;
            err += dy * 2 + 1;
        }
        if (err > 0) {
            dx --;
            err -= dx * 2 + 1;
        }
    }
}

SDL_Texture *Painter::loadTexture(string path) {
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << "\n";
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << "\n";
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool Painter::createImage(SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *dstrect) {
    if (texture == NULL) return false;
    SDL_RenderCopy(renderer, texture, NULL, dstrect);
    return true;
}

