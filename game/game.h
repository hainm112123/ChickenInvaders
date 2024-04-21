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
#include "slider.h"

const double BOSS_TURN_DELAY = 1.2;
const double MINI_BOSS_TIME_LIMIT = 60;

const int ROCK_FALL_WAVE = 15;
const int ROCK_SIDE_WAVE = 10;
const int NG_ROCK_WAVE = 3;
const double ROCK_WAVE_DELAY = 1.2;

enum GameRound {
    INIT_ROUND = 0,
    CHICKEN_ROUND,
    CHICKEN_CIRCULAR_ROUND,
    ROCK_SIDE_ROUND,
    MINI_BOSS_ROUND,
    ROCK_FALL_ROUND,
    BOSS_ROUND,
};
const int ROUND_COUNT = 6;
const int ROUND_SCORE[] = {100, 150, 400, 500, 400, 600};
const int NG_ROUND_SCORE[] = {120, 144, 169, 196, 169, 256};
const int NUMBER_OF_CHICKEN[] = {0, 30, 20, 0, 1, 0, 1};
const int NUMBER_OF_CHICKEN_PER_ROW[] = {0, 10, 1, 0, 1, 0, 1};
const int NUMBER_OF_CHICKEN_BULLET[] = {0, 10, 0, 0, 0, 0, 2};

const double SCREEN_SPEED = 144;
const int BG_SIZE = 762;

const double INIT_DELAY = 3.6;
const double GAME_END_DELAY = 3;
const double GUNDAM_REVIVE_TIME = 2;
const double GUNDAM_SHIELD_DURATION = 3;
const double GUNDAM_LASER_DURATION = 15;

const int CHICKENS_TO_LEVEL_UP = 30;
const int BOSS_TO_NEW_WEAPON = 1;
const int FRIED_CHICKEN_TO_ROCKET = 30;

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
    GAME_INSANE,
    GAME_DIFFICULTY_COUNT,
};
const string GAME_DIFFICULTY[] = {"Easy", "Normal", "Hard", "Insane"};

enum GameAudio {
    AUDIO_UNMUTED = 0,
    AUDIO_MUTED,
    AUDIO_COUNT,
};
const string GAME_AUDIO[] = {"Unmuted", "Muted"};

enum MenuState {
    MENU_MAIN = 0,
    MENU_PLAY,
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
enum PlayMenuTab {
    PLAY_MENU_NEW_GAME = 1,
    PLAY_MENU_CONTINUE
};
enum SettingsMenuTab {
    SETTING_MENU_DIFFICULTY = 1,
    SETTING_MENU_AUDIO,
};
enum RankingMenuTab {
    RANKING_MENU_CLEAR_RANKING = 1,
};

const string ROUND_TEXT[] = {
    "",
    "Warm up!",
    "Get ready!",
    "Asteroid is coming!",
    "Be quick or dead!",
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

struct LastGameData {
    int score, NG;
    GameRound round;
    GameDifficulty difficultyState;
};

class Game {
    SDL_Renderer *renderer;
    SDL_Event *event;
    TTF_Font *fontMenu, *fontMenuHover, *fontGame, *fontRoundTitle, *fontRoundText;

    bool roundWon;
    int width, height;
    int score, NG = 0, game_difficulty;
    GameRound round;
    GameStatus status;
    GameDifficulty difficultyState;
    GameAudio audioState;
    Entity background;
    double scrolling = 0;

    Timer initTimer, gameEndTimer, rockWaveTimer, bossTurnTimer;
//    Timer gundamReviveTimer, gundamShieldTimer, gundamLaserTimer;

    Text roundTitle, roundText;
    Text playerNames[NUMBER_SHOWED_PLAYER], playerScores[NUMBER_SHOWED_PLAYER];

    Gundam gundam;
    Rocket rocket;
    double rocket_damage;

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
    Timer roundTimer;
    Text roundTimerText;

    set<Upgrade*> upgrades;
    deque<Entity*> explosions;

    int rocketCount = 0, frychickenCount = 0;
    map<string, int> scores;
    vector<Entity> hearts;
    Entity gundamLevelImage, rocketMini, frychickenMini;
    Text scoreText, scoreValue, gundamLevelText, frychickenText, rocketText;

    Entity pause_menu, home_button, audio_button, resume_button, pause_button;

    MenuState menuState = MENU_MAIN;
    Entity menu, menu_play, menu_settings, menu_control;
    vector<Text> settingsMenuText;
    vector<Text> mainMenuChoice, playMenuChoice, settingsMenuChoice, rankingMenuChoice, controlMenuChoice;
    Text backButton;
    Slider SFX, BGM;

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
    GameDifficulty DifficultyState() const {
        return difficultyState;
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
    void gundamDead(bool immediately = false);
    void chickenDead(Chicken *chicken);
    void chickenReceiveDamage(Chicken *chicken, double damage);
    void rockReceiveDamage(Rock *rock, double damage);

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
