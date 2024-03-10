#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../init.h"
#include "../gundam/gundam.h"
#include "../Painter.h"
#include "../enemy/chicken.h"
#include "../gundam/upgrade.h"
#include "../enemy/rock.h"
#include "text.h"

const int NUMBER_OF_CHICKEN = 30;
const int NUMBER_OF_CHICKEN_PER_ROW = 10;
const int ROCK_FALL_WAVE = 5;
const int ROCK_SIDE_WAVE = 15;
const int ROCK_WAVE_FRAME = 100;

const int ROCK_FALL_ROUND = 5;
const int ROCK_SIDE_ROUND = 3;
const int MINI_BOSS_ROUND = 4;
const int BOSS_ROUND = 6;
const int ROUND_COUNT = 6;

const int UPGRADE_SPEED = 1;
const int SCREEN_SPEED = 1;
const int BG_SIZE = 762;

const double INIT_DELAY = 3;
const double GUNDAM_REVIVE_TIME = 2;
const double GUNDAM_SHIELD_DURATION = 3;
const double GUNDAM_LASER_DURATION = 10;

enum GameStatus {
    GAME_STOP = 0,
    GAME_RUNNING,
    GAME_OVER,
    GAME_WON,
};

enum GameDifficulty {
    GAME_EASY = 0,
    GAME_NORMAL,
    GAME_HARD,
    GAME_DIFFICULTY_COUNT,
};
const string GAME_DIFFICULTY[] = {"Easy", "Normal", "Hard"};

enum GameAudio {
    AUDIO_UNMUTED = 0,
    AUDIO_MUTED,
    AUDIO_COUNT,
};
const string GAME_AUDIO[] = {"Unmuted", "Muted"};

enum MenuState {
    MENU_MAIN = 0,
    MENU_SETTINGS,
};
enum MainMenuTab {
    MAIN_MENU_START = 0,
    MAIN_MENU_SETTINGS,
    MAIN_MENU_QUIT,
};
enum SettingsMenuTab {
    SETTING_MENU_AUDIO = 0,
    SETTING_MENU_DIFFICULTY
};

const string ROUND_TEXT[] = {
    "",
    "Warm up!",
    "Get ready!",
    "Asteroid is coming!",
    "Get ready",
    "High-speed asteroid!",
    "Be careful!",
};

class Game {
    SDL_Renderer *renderer;
    SDL_Event *event;
    Painter *painter;
    Gallery *gallery;
    Media *media;
    TTF_Font *fontMenu, *fontGame, *fontRoundTitle, *fontRoundText;

    bool roundWon;
    int width, height;
    int score, round, NG = 0;
    GameStatus status;
    GameDifficulty difficultyState;
    GameAudio audioState;
    Entity background;
    int scrolling = 0;
    int frame = 0;
    Timer initTimer, gundamReviveTimer, gundamShieldTimer, gundamLaserTimer;
    Text roundTitle, roundText;

    Gundam gundam;

    vector<Chicken*> chickens;
    int numberOfAliveChicken;
    Chicken *topChicken, *bottomChicken, *leftChicken, *rightChicken;
    vector<Bullet*>chickenBullets;
    ChickenMoveState chickenMoveState;
    vector<int> killedChickenCount;
    set<Rock*> rocks;

    set<Upgrade*> upgrades;
    deque<Entity*> explosions;

public:
    Game(SDL_Renderer *_renderer, SDL_Event *_event, Painter *_painter, int _width, int _height);
    ~Game();

    int getWidth() const {
        return width;
    }
    int getHeight() const {
        return height;
    }
    int getGameStatus() const {
        return status;
    }
    Gallery* getGallery() const {
        return gallery;
    }
    Media* getMedia() const {
        return media;
    }

    void setGameStatus(GameStatus newStatus);
    void load();
    void init();
    void renderMenu();
    void handleGameEvent();
    void process();
    void quit();

    void setRoundWon();
    void dropUpgrade(EntityType type);
    void addExplosion(SDL_Rect rect);
    void gundamDead();
    void chickenDead(Chicken *chicken);
    void playChunk(Mix_Chunk *chunk, int channel = -1, int loop = 0);
    void playMusic(Mix_Music *music);
};

#endif // GAME_H_INCLUDED
