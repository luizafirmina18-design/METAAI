#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>

// ===== CONFIG DA TELA =====
#define SCREEN_W 1280
#define SCREEN_H 720
#define WORLD_RADIUS 4000.0f
#define MAX_SNAKES 26
#define MAX_FOOD 500
#define MAX_PARTICLES 300

// ===== ESTRUTURAS =====
struct Vec2 {
    float x;
    float y;
    Vec2() : x(0), y(0) {}
    Vec2(float _x, float _y) : x(_x), y(_y) {}
};

struct Food {
    Vec2 pos;
    float radius;
    Uint8 r, g, b;
    float pulse;
    bool active;
};

struct Particle {
    Vec2 pos;
    Vec2 vel;
    float life;
    float maxLife;
    Uint8 r, g, b;
    float size;
    bool active;
};

struct Snake {
    bool isPlayer;
    bool alive;
    std::vector<Vec2> path;
    std::vector<Vec2> body;
    float angle;
    float targetAngle;
    float speed;
    float baseSpeed;
    float boostSpeed;
    float radius;
    float length;
    int score;
    int kills;
    Uint8 colorR, colorG, colorB;
    Uint8 colorR2, colorG2, colorB2;
    float boostTimer;
    bool boosting;
    float turnSpeed;
    int aiTimer;
    Vec2 aiTarget;
    bool aiFlee;
    std::string name;
};

// ===== CLASSE PRINCIPAL =====
class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
    void shutdown();

private:
    SDL_Surface* screen;
    bool running;
    int gameState; // 0 = MENU, 1 = JOGANDO, 2 = GAME OVER
    float cameraX;
    float cameraY;
    float zoom;
    float targetZoom;
    int frameCount;

    Snake snakes[MAX_SNAKES];
    Food foods[MAX_FOOD];
    Particle particles[MAX_PARTICLES];

    // Métodos principais
    void initWorld();
    void resetPlayer();
    void spawnSnake(Snake &s, bool isPlayer, float x, float y);
    void spawnFood(int index, bool anywhere);
    void spawnParticle(float x, float y, Uint8 r, Uint8 g, Uint8 b);
    void update(float dt);
    void updatePlayer(Snake &s, float dt, float analogX, float analogY, bool boostBtn);
    void updateBot(Snake &s, float dt);
    void updateFoods(float dt);
    void updateParticles(float dt);
    void updateCamera(float dt);
    void checkCollisions();
    void eatFood(Snake &s, int foodIndex);
    void killSnake(int index, int killerIndex);
    void addScore(Snake &s, int amount);

    // Desenho
    void render();
    void renderMenu();
    void renderGameOver();
    void drawBackground();
    void drawBorder();
    void drawFood();
    void drawSnake(Snake &s);
    void drawParticles();
    void drawMinimap();
    void drawHUD();
    void drawGlowCircle(int sx, int sy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha);
    void drawFilledCircle(int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b);
    void drawCircleOutline(int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b);
    void drawText(const char* text, int x, int y, Uint8 r, Uint8 g, Uint8 b, bool big);

    // Utilitários
    float dist(float x1, float y1, float x2, float y2);
    float angleTo(float x1, float y1, float x2, float y2);
    float normalizeAngle(float a);
    Vec2 worldToScreen(float wx, float wy);
    bool isInsideWorld(float x, float y);
    Uint32 getColor(Uint8 r, Uint8 g, Uint8 b);
    void setPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b);
};

#endif