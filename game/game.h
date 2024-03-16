#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include<list>

#include "../common/common.h"
#include "../gundam/gundam.h"
#include "../enemy/chicken.h"
#include "../gundam/upgrade.h"
#include "../enemy/rock.h"
#include "text.h"
#include "../enemy/fried_chicken.h"
#include "../weapon/rocket.h"

const int NUMBER_OF_CHICKEN = 30;
const int NUMBER_OF_CHICKEN_PER_ROW = 10;
const double BOSS_TURN_DELAY = 1.2;

const int ROCK_FALL_WAVE = 15;
const int ROCK_SIDE_WAVE = 10;
const int NG_ROCK_WAVE = 10;
const double ROCK_WAVE_DELAY = 0.87654321;

const int ROCK_FALL_ROUND = 5;
const int ROCK_SIDE_ROUND = 3;
const int MINI_BOSS_ROUND = 4;
const int BOSS_ROUND = 6;
const int ROUND_COUNT = 6;
const int ROUND_SCORE[] = {100, 150, 400, 250, 400, 300};
const int NG_ROUND_SCORE[] = {36, 49, 100, 144, 121, 256};

const double SCREEN_SPEED = 144;
const int BG_SIZE = 762;

const double INIT_DELAY = 3;
const double GAME_END_DELAY = 3;
const double GUNDAM_REVIVE_TIME = 2;
const double GUNDAM_SHIELD_DURATION = 3;
const double GUNDAM_LASER_DURATION = 15;

const int CHICKENS_TO_LEVEL_UP = 30;
const int BOSS_TO_NEW_WEAPON = 1;
const int FRIED_CHICKEN_TO_ROCKET = 10;

enum GameStatus {
    GAME_STOP = 0,
    GAME_INITALIZING,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_OVER,
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
    MENU_CONTROL,
    MENU_RANKING,
};
enum MainMenuTab {
    MAIN_MENU_START = 0,
    MAIN_MENU_SETTINGS,
    MAIN_MENU_CONTROL,
    MAIN_MENU_RANKING,
    MAIN_MENU_QUIT,
};
enum SettingsMenuTab {
    SETTING_MENU_AUDIO = 1,
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

const SDL_Keycode KEYCODES[] = {
    SDLK_a,
    SDLK_b,
    SDLK_c,
    SDLK_d,
    SDLK_e,
    SDLK_f,
    SDLK_g,
    SDLK_h,
    SDLK_i,
    SDLK_j,
    SDLK_k,
    SDLK_l,
    SDLK_m,
    SDLK_n,
    SDLK_o,
    SDLK_p,
    SDLK_q,
    SDLK_r,
    SDLK_s,
    SDLK_t,
    SDLK_u,
    SDLK_v,
    SDLK_w,
    SDLK_x,
    SDLK_y,
    SDLK_z,
    SDLK_0,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_5,
    SDLK_6,
    SDLK_7,
    SDLK_8,
    SDLK_9,
};
const int NUM_KEYCODES = 36;
const int MAX_NAME_LENGTH = 16;
const int NUMBER_SHOWED_PLAYER = 10;

class Game {
    SDL_Renderer *renderer;
    SDL_Event *event;
    TTF_Font *fontMenu, *fontGame, *fontRoundTitle, *fontRoundText;

    bool roundWon;
    int width, height;
    int score, round, NG = 0, game_difficulty;
    GameStatus status;
    GameDifficulty difficultyState;
    GameAudio audioState;
    Entity background;
    double scrolling = 0;
    Timer initTimer, gameEndTimer, rockWaveTimer, bossTurnTimer;
    Timer gundamReviveTimer, gundamShieldTimer, gundamLaserTimer;
    Text roundTitle, roundText;
    Text playerNames[NUMBER_SHOWED_PLAYER], playerScores[NUMBER_SHOWED_PLAYER];

    Gundam gundam;
    Rocket rocket;

    vector<Chicken*> chickens;
    int numberOfAliveChicken;
    Chicken *topChicken, *bottomChicken, *leftChicken, *rightChicken;
    vector<Bullet*>chickenBullets;
    vector<int> killedChickenCount;
    set<Rock*> rocks;
    Entity bossHealthBar, bossHealthBorder;
    int bossHP = 0;
    int rockWaveCount = 0;
    vector<pair<double, double>>enemy_positions;
    list<FriedChicken*> fried_chickens;

    set<Upgrade*> upgrades;
    deque<Entity*> explosions;

    int rocketCount = 0, frychickenCount = 0;
    map<string, int> scores;
    vector<Entity> hearts;
    Entity gundamLevelImage, rocketMini, frychickenMini;
    Text scoreText, scoreValue, gundamLevelText, frychickenText, rocketText;

    Entity pause_button, home_button, audio_button, resume_button, pause_menu;
    SDL_Texture *overlay;

public:
    Game(SDL_Renderer *_renderer, SDL_Event *_event, int _width, int _height);
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
    bool isRunning() const {
        return status != GAME_STOP;
    }

    void setGameStatus(GameStatus newStatus);
    void load();
    void renderMenu();
    void renderPauseMenu();

    void init();
    void init_rock();
    void handleGameEvent();
    void process();
    void process_game_state();
    void process_enemy();
    void process_gundam();
    void quit();

    void setRoundWon();
    void dropUpgrade(EntityType type);
    void addExplosion(SDL_Rect rect, int level);
    void addFriedChicken(double x, double y, int level);
    void gundamDead();
    void chickenDead(Chicken *chicken);
    void playChunk(Mix_Chunk *chunk, int channel = -1, int loop = 0);
    void playMusic(Mix_Music *music);
    void toggleAudio();

    void removeUpgrade(Upgrade *upgrade);
    void removeFriedChicken(FriedChicken *fried_chicken);
    void removeRock(Rock *rock);

    void gameOver();
    void enterYourName();
    void playAgain();
    void reset();
    void initData();
    void saveData();
    void showRanking();
    vector<pair<int, string>> getRanking();

    void _clear(bool round_init = false);
};

#endif // GAME_H_INCLUDED
