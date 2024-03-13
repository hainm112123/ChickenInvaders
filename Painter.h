#ifndef PAINTER_H_INCLUDED
#define PAINTER_H_INCLUDED

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "common/common.h"

using namespace std;

const SDL_Color WHITE_COLOR = {255, 255, 255, 255};
const SDL_Color WHITE_COLOR_2ND = {255, 255, 255, 0};
const SDL_Color BLACK_COLOR = {0, 0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0, 0};
const SDL_Color RED_COLOR = {255, 0, 0, 0};
const SDL_Color TEXT_COLOR = {255, 255, 255, 0};
const SDL_Color TEXT_HOVER_COLOR = {128, 128, 128, 0};
const SDL_Color TEXT_PLACEHOLDER_COLOR = {128, 128, 128, 0};

class Painter {
    double x;
    double y;
    double angle;
    SDL_Color color;
    SDL_Renderer *renderer;

    public:
        Painter(SDL_Window *window, SDL_Renderer *renderer);

        void setPosition (double x, double y);
        double getX() const {
            return x;
        }
        double getY() const {
            return y;
        }

        void setAngle(double angle);
        double getAngle() const {
            return angle;
        }

        void setColor(SDL_Color color);
        SDL_Color getColor() const {
            return color;
        }

        void clearWithBgColor(SDL_Color bgColor);

        void moveForward(double length);
        void jumpForward(double length);
        void moveBackward(double length) {
            moveForward(-length);
        }
        void jumpBackward(double length) {
            jumpForward(-length);
        }
        void turnLeft(double angle) {
            setAngle(this->angle + angle);
        }
        void turnRight(double angle) {
            turnLeft(-angle);
        }

        void createSquare(double _size);
        void createCircle(double radius);

        SDL_Renderer* getRenderer() const {
            return renderer;
        }

        Texture loadTexture(string path);
        bool createImage(SDL_Texture *texture, SDL_Rect *srcrect = nullptr, SDL_Rect *dstrect = nullptr);
};

#endif // PAINTER_H_INCLUDED
