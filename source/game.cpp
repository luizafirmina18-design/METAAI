#include "game.h"
#include <io/pad.h>
#include <sysutil/sysutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Garante que o body tenha pelo menos 'length' segmentos.
// Quando a cobra cresce, o length aumenta mas o body não, e isso fazia
// o código acessar posições inexistentes e travar o PS3.
static void ensureBody(Snake &s) {
    while ((int)s.body.size() < (int)s.length) {
        s.body.push_back(s.body.empty() ? Vec2(0, 0) : s.body.back());
    }
}

// ===== CONSTRUTOR / DESTRUTOR =====
Game::Game() {
    screen = NULL;
    running = false;
    gameState = 0;
    cameraX = 0;
    cameraY = 0;
    zoom = 1.0f;
    targetZoom = 1.0f;
    frameCount = 0;
    for (int i = 0; i < MAX_FOOD; i++) foods[i].active = false;
    for (int i = 0; i < MAX_PARTICLES; i++) particles[i].active = false;
}

Game::~Game() {
    shutdown();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        return false;
    }

    // Tenta usar a maior resolução que o console listar
    int w = SCREEN_W;
    int h = SCREEN_H;
    SDL_Rect **modes = SDL_ListModes(NULL, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (modes && modes != (SDL_Rect **)-1 && modes[0]) {
        w = modes[0]->w;
        h = modes[0]->h;
    }

    screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        // Se falhar, volta para a resolução padrão do jogo
        screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        if (!screen) return false;
    }

    SDL_ShowCursor(SDL_DISABLE);
    ioPadInit(7);
    srand(12345);
    initWorld();
    running = true;
    return true;
}

void Game::shutdown() {
    ioPadEnd();
    SDL_Quit();
    screen = NULL;
}

// ===== MUNDO =====
void Game::initWorld() {
    for (int i = 0; i < MAX_SNAKES; i++) {
        snakes[i].alive = false;
        snakes[i].path.clear();
        snakes[i].body.clear();
    }
    // Player no centro
    spawnSnake(snakes[0], true, 0, 0);
    // Bots espalhados
    for (int i = 1; i < MAX_SNAKES; i++) {
        float a = ((float)rand() / RAND_MAX) * 6.283f;
        float d = 500 + ((float)rand() / RAND_MAX) * 3000;
        spawnSnake(snakes[i], false, cosf(a) * d, sinf(a) * d);
    }
    // Comidas
    for (int i = 0; i < MAX_FOOD; i++) {
        spawnFood(i, true);
    }
    cameraX = snakes[0].body[0].x;
    cameraY = snakes[0].body[0].y;
    zoom = 1.0f;
    targetZoom = 1.0f;
    gameState = 0;
}

void Game::resetPlayer() {
    initWorld();
    gameState = 1;
}

void Game::spawnSnake(Snake &s, bool isPlayer, float x, float y) {
    s.isPlayer = isPlayer;
    s.alive = true;
    s.angle = ((float)rand() / RAND_MAX) * 6.283f;
    s.targetAngle = s.angle;
    s.baseSpeed = 2.8f;
    s.boostSpeed = 5.6f;
    s.speed = s.baseSpeed;
    s.boosting = false;
    s.boostTimer = 0;
    s.turnSpeed = 0.08f;
    s.aiTimer = 0;
    s.aiTarget = Vec2(x + 200, y);
    s.aiFlee = false;
    s.score = 0;
    s.kills = 0;

    if (isPlayer) {
        s.length = 25;
        s.radius = 14;
        s.colorR = 0; s.colorG = 200; s.colorB = 80;
        s.colorR2 = 0; s.colorG2 = 150; s.colorB2 = 50;
        s.name = "VOCE";
    } else {
        s.length = 12 + rand() % 60;
        s.radius = 10 + (rand() % 8);
        int c = rand() % 6;
        if (c == 0) { s.colorR = 255; s.colorG = 50;  s.colorB = 50;  s.colorR2 = 200; s.colorG2 = 20;  s.colorB2 = 20; }
        else if (c == 1) { s.colorR = 50;  s.colorG = 150; s.colorB = 255; s.colorR2 = 20;  s.colorG2 = 100; s.colorB2 = 220; }
        else if (c == 2) { s.colorR = 255; s.colorG = 200; s.colorB = 0;   s.colorR2 = 220; s.colorG2 = 150; s.colorB2 = 0; }
        else if (c == 3) { s.colorR = 200; s.colorG = 50;  s.colorB = 255; s.colorR2 = 150; s.colorG2 = 20;  s.colorB2 = 220; }
        else if (c == 4) { s.colorR = 0;   s.colorG = 220; s.colorB = 220; s.colorR2 = 0;   s.colorG2 = 170; s.colorB2 = 170; }
        else { s.colorR = 255; s.colorG = 120; s.colorB = 0;   s.colorR2 = 220; s.colorG2 = 80;  s.colorB2 = 0; }
        s.name = "BOT";
    }

    s.path.clear();
    s.body.clear();
    for (int i = 0; i < (int)s.length * 2; i++) {
        float px = x - cosf(s.angle) * i * 8;
        float py = y - sinf(s.angle) * i * 8;
        s.path.push_back(Vec2(px, py));
    }
    for (int i = 0; i < (int)s.length; i++) {
        int idx = i * 2;
        if (idx < (int)s.path.size()) s.body.push_back(s.path[idx]);
        else s.body.push_back(Vec2(x, y));
    }
}

void Game::spawnFood(int index, bool anywhere) {
    (void)anywhere;
    float a = ((float)rand() / RAND_MAX) * 6.283f;
    float d = sqrtf((float)rand() / RAND_MAX) * (WORLD_RADIUS - 100);
    foods[index].pos = Vec2(cosf(a) * d, sinf(a) * d);
    foods[index].radius = 4 + (rand() % 7);
    if (rand() % 10 == 0) foods[index].radius = 12;
    int c = rand() % 7;
    if (c == 0) { foods[index].r = 255; foods[index].g = 50;  foods[index].b = 80; }
    else if (c == 1) { foods[index].r = 255; foods[index].g = 200; foods[index].b = 0; }
    else if (c == 2) { foods[index].r = 0;   foods[index].g = 220; foods[index].b = 100; }
    else if (c == 3) { foods[index].r = 0;   foods[index].g = 180; foods[index].b = 255; }
    else if (c == 4) { foods[index].r = 200; foods[index].g = 0;   foods[index].b = 255; }
    else if (c == 5) { foods[index].r = 255; foods[index].g = 120; foods[index].b = 0; }
    else { foods[index].r = 255; foods[index].g = 255; foods[index].b = 255; }
    foods[index].pulse = ((float)rand() / RAND_MAX) * 6.28f;
    foods[index].active = true;
}

void Game::spawnParticle(float x, float y, Uint8 r, Uint8 g, Uint8 b) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) {
            particles[i].pos = Vec2(x, y);
            float a = ((float)rand() / RAND_MAX) * 6.283f;
            float s = 1 + ((float)rand() / RAND_MAX) * 3;
            particles[i].vel = Vec2(cosf(a) * s, sinf(a) * s);
            particles[i].life = 1.0f;
            particles[i].maxLife = 1.0f;
            particles[i].r = r; particles[i].g = g; particles[i].b = b;
            particles[i].size = 2 + rand() % 4;
            particles[i].active = true;
            return;
        }
    }
}

// ===== LOOP PRINCIPAL =====
void Game::run() {
    padInfo padinfo;
    padData paddata;
    Uint32 lastTime = SDL_GetTicks();
    bool startPressedBefore = false;

    while (running) {
        Uint32 frameStart = SDL_GetTicks();
        float dt = (frameStart - lastTime) / 16.666f;
        if (dt > 3) dt = 3;
        if (dt < 0.1f) dt = 0.1f;
        lastTime = frameStart;

        sysUtilCheckCallback();

        // Leitura do controle
        float analogX = 0, analogY = 0;
        bool boostBtn = false;
        bool startBtn = false;
        ioPadGetInfo(&padinfo);
        if (padinfo.status[0]) {
            ioPadGetData(0, &paddata);
            int ax = paddata.ANA_L_H - 128;
            int ay = paddata.ANA_L_V - 128;
            if (ax < -10 || ax > 10 || ay < -10 || ay > 10) {
                analogX = ax / 128.0f;
                analogY = ay / 128.0f;
                if (analogX > 1) analogX = 1;
                if (analogX < -1) analogX = -1;
                if (analogY > 1) analogY = 1;
                if (analogY < -1) analogY = -1;
            }
            if (paddata.BTN_CROSS || paddata.BTN_R2) boostBtn = true;
            if (paddata.BTN_START) startBtn = true;
        }

        if (gameState == 0) {                       // MENU
            if (startBtn && !startPressedBefore) gameState = 1;
        } else if (gameState == 1) {                // JOGANDO
            for (int i = 0; i < MAX_SNAKES; i++) {
                if (snakes[i].alive) ensureBody(snakes[i]);
            }
            updatePlayer(snakes[0], dt, analogX, analogY, boostBtn);
            for (int i = 1; i < MAX_SNAKES; i++) {
                if (snakes[i].alive) updateBot(snakes[i], dt);
            }
            // Move todas as cobras pelo path
            for (int i = 0; i < MAX_SNAKES; i++) {
                if (!snakes[i].alive) continue;
                Snake &s = snakes[i];
                ensureBody(s);
                s.path.insert(s.path.begin(), s.body[0]);
                if ((int)s.path.size() > (int)s.length * 2 + 5) {
                    s.path.pop_back();
                }
                for (int j = 0; j < (int)s.length; j++) {
                    int idx = j * 2;
                    if (idx < (int)s.path.size()) s.body[j] = s.path[idx];
                }
            }
            updateFoods(dt);
            updateParticles(dt);
            updateCamera(dt);
            checkCollisions();
            if (!snakes[0].alive) gameState = 2;
        } else if (gameState == 2) {                // GAME OVER
            if (startBtn && !startPressedBefore) resetPlayer();
        }
        startPressedBefore = startBtn;

        render();
        frameCount++;

        // Trava em ~60 FPS
        Uint32 elapsed = SDL_GetTicks() - frameStart;
        if (elapsed < 16) SDL_Delay(16 - elapsed);
    }
}

void Game::update(float dt) {
    (void)dt;
}

void Game::updatePlayer(Snake &s, float dt, float analogX, float analogY, bool boostBtn) {
    if (!s.alive) return;
    if (analogX != 0 || analogY != 0) {
        s.targetAngle = atan2f(analogY, analogX);
    }
    float diff = normalizeAngle(s.targetAngle - s.angle);
    float maxTurn = s.turnSpeed * dt;
    if (diff > maxTurn) s.angle += maxTurn;
    else if (diff < -maxTurn) s.angle -= maxTurn;
    else s.angle = s.targetAngle;

    s.boosting = boostBtn && s.length > 20;
    s.speed = s.boosting ? s.boostSpeed : s.baseSpeed;

    s.body[0].x += cosf(s.angle) * s.speed * dt;
    s.body[0].y += sinf(s.angle) * s.speed * dt;

    if (s.boosting) {
        s.boostTimer += dt;
        if (s.boostTimer > 8) {
            s.boostTimer = 0;
            if (s.length > 20) {
                s.length -= 0.5f;
                int tail = (int)s.length - 1;
                if (tail < 0) tail = 0;
                if (tail >= (int)s.body.size()) tail = (int)s.body.size() - 1;
                Vec2 dropAt = s.body[tail];
                for (int i = 0; i < MAX_FOOD; i++) {
                    if (!foods[i].active) {
                        foods[i].pos = dropAt;
                        foods[i].radius = 5;
                        foods[i].r = s.colorR;
                        foods[i].g = s.colorG;
                        foods[i].b = s.colorB;
                        foods[i].active = true;
                        foods[i].pulse = 0;
                        break;
                    }
                }
            }
        }
        spawnParticle(s.body[0].x, s.body[0].y, s.colorR, s.colorG, s.colorB);
        spawnParticle(s.body[0].x, s.body[0].y, 255, 255, 255);
    }

    if (!isInsideWorld(s.body[0].x, s.body[0].y)) {
        killSnake(0, -1);
    }
}

void Game::updateBot(Snake &s, float dt) {
    if (!s.alive) return;
    s.aiTimer -= 1;   // antes era (int)dt, que virava 0 e travava a IA
    Vec2 head = s.body[0];

    float dCenter = dist(head.x, head.y, 0, 0);
    if (dCenter > WORLD_RADIUS - 400) {
        s.aiTarget = Vec2(0, 0);
        s.aiFlee = true;
        s.aiTimer = 30;
    } else if (s.aiTimer <= 0) {
        s.aiFlee = false;
        float best = 999999;
        Vec2 bestFood = head;
        for (int i = 0; i < MAX_FOOD; i++) {
            if (!foods[i].active) continue;
            float d = dist(head.x, head.y, foods[i].pos.x, foods[i].pos.y);
            float score = d - foods[i].radius * 20;
            if (score < best) { best = score; bestFood = foods[i].pos; }
        }
        s.aiTarget = bestFood;
        s.aiTimer = 40 + rand() % 60;
        if (rand() % 100 < 15) {
            s.aiTarget = snakes[0].body[0];
        }
    }

    // Desvia de cobra na frente
    bool danger = false;
    float dangerAngle = 0;
    for (int i = 0; i < MAX_SNAKES; i++) {
        if (!snakes[i].alive) continue;
        Snake &other = snakes[i];
        int limit = (int)other.length;
        if (limit > (int)other.body.size()) limit = (int)other.body.size();
        for (int j = 5; j < limit; j++) {
            float d = dist(head.x + cosf(s.angle) * 120, head.y + sinf(s.angle) * 120,
                           other.body[j].x, other.body[j].y);
            if (d < other.radius + 40) {
                danger = true;
                dangerAngle = angleTo(head.x, head.y, other.body[j].x, other.body[j].y);
                break;
            }
        }
        if (danger) break;
    }

    if (danger) {
        s.targetAngle = dangerAngle + 1.57f;
        if (rand() % 2 == 0) s.targetAngle = dangerAngle - 1.57f;
    } else {
        s.targetAngle = angleTo(head.x, head.y, s.aiTarget.x, s.aiTarget.y);
    }

    float diff = normalizeAngle(s.targetAngle - s.angle);
    float maxTurn = s.turnSpeed * dt;
    if (diff > maxTurn) s.angle += maxTurn;
    else if (diff < -maxTurn) s.angle -= maxTurn;
    else s.angle = s.targetAngle;

    s.boosting = (rand() % 1000 < 8) && s.length > 30;
    s.speed = s.boosting ? s.boostSpeed : s.baseSpeed;

    s.body[0].x += cosf(s.angle) * s.speed * dt;
    s.body[0].y += sinf(s.angle) * s.speed * dt;

    if (s.boosting) {
        spawnParticle(head.x, head.y, s.colorR, s.colorG, s.colorB);
        if (s.length > 20) s.length -= 0.02f;
    }
    if (!isInsideWorld(s.body[0].x, s.body[0].y)) {
        int idx = -1;
        for (int i = 0; i < MAX_SNAKES; i++) if (&snakes[i] == &s) idx = i;
        killSnake(idx, -1);
    }
}

void Game::updateFoods(float dt) {
    for (int i = 0; i < MAX_FOOD; i++) {
        if (!foods[i].active) continue;
        foods[i].pulse += 0.05f * dt;
    }
}

void Game::updateParticles(float dt) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        particles[i].pos.x += particles[i].vel.x * dt;
        particles[i].pos.y += particles[i].vel.y * dt;
        particles[i].life -= 0.03f * dt;
        if (particles[i].life <= 0) particles[i].active = false;
    }
}

void Game::updateCamera(float dt) {
    if (!snakes[0].alive) return;
    Vec2 head = snakes[0].body[0];
    // Usa a resolução real da tela
    float tx = head.x - (screen->w / 2) / zoom;
    float ty = head.y - (screen->h / 2) / zoom;
    cameraX += (tx - cameraX) * 0.08f * dt;
    cameraY += (ty - cameraY) * 0.08f * dt;
    targetZoom = 1.0f - (snakes[0].length / 400.0f);
    if (targetZoom < 0.45f) targetZoom = 0.45f;
    zoom += (targetZoom - zoom) * 0.02f * dt;
}

void Game::checkCollisions() {
    // Cobras comem comida
    for (int s = 0; s < MAX_SNAKES; s++) {
        if (!snakes[s].alive) continue;
        Vec2 head = snakes[s].body[0];
        for (int f = 0; f < MAX_FOOD; f++) {
            if (!foods[f].active) continue;
            float d = dist(head.x, head.y, foods[f].pos.x, foods[f].pos.y);
            if (d < snakes[s].radius + foods[f].radius) {
                eatFood(snakes[s], f);
            }
        }
    }
    // Cabeça bate no corpo dos outros
    for (int s = 0; s < MAX_SNAKES; s++) {
        if (!snakes[s].alive) continue;
        Vec2 head = snakes[s].body[0];
        for (int o = 0; o < MAX_SNAKES; o++) {
            if (o == s) continue;
            if (!snakes[o].alive) continue;
            int limit = (int)snakes[o].length;
            if (limit > (int)snakes[o].body.size()) limit = (int)snakes[o].body.size();
            for (int j = 8; j < limit; j++) {
                float d = dist(head.x, head.y, snakes[o].body[j].x, snakes[o].body[j].y);
                if (d < snakes[s].radius * 0.7f + snakes[o].radius * 0.7f) {
                    killSnake(s, o);
                    break;
                }
            }
            if (!snakes[s].alive) break;
        }
    }
}

void Game::eatFood(Snake &s, int foodIndex) {
    foods[foodIndex].active = false;
    s.length += 0.6f;
    ensureBody(s);
    s.score += (int)foods[foodIndex].radius;
    addScore(s, (int)foods[foodIndex].radius);
    spawnFood(foodIndex, true);
    if (s.length > 80 && s.radius < 22) s.radius = 14 + s.length / 80;
}

void Game::killSnake(int index, int killerIndex) {
    if (index < 0 || index >= MAX_SNAKES) return;
    Snake &s = snakes[index];
    if (!s.alive) return;
    s.alive = false;
    ensureBody(s);

    // Solta comida em cada segmento da cobra morta.
    // Se não houver slot livre, recicla uma comida qualquer,
    // para que a comida da cobra sempre apareça.
    int n = (int)s.body.size();
    for (int j = 0; j < n; j += 2) {
        int slot = -1;
        for (int f = 0; f < MAX_FOOD; f++) {
            if (!foods[f].active) { slot = f; break; }
        }
        if (slot < 0) slot = rand() % MAX_FOOD;

        foods[slot].pos = s.body[j];
        foods[slot].radius = 7;
        foods[slot].r = s.colorR;
        foods[slot].g = s.colorG;
        foods[slot].b = s.colorB;
        foods[slot].pulse = 0;
        foods[slot].active = true;
    }

    for (int i = 0; i < 20; i++) {
        spawnParticle(s.body[0].x, s.body[0].y, s.colorR, s.colorG, s.colorB);
    }

    if (killerIndex >= 0 && killerIndex < MAX_SNAKES) {
        snakes[killerIndex].kills++;
        snakes[killerIndex].score += 100;
    }

    // Bot morto respawna longe do player
    if (index != 0) {
        float a = ((float)rand() / RAND_MAX) * 6.283f;
        float d = 500 + ((float)rand() / RAND_MAX) * 3000;
        spawnSnake(snakes[index], false, cosf(a) * d, sinf(a) * d);
    }
}

void Game::addScore(Snake &s, int amount) {
    (void)amount;
    s.score += 1;
}

// ===== UTILITÁRIOS =====
float Game::dist(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

float Game::angleTo(float x1, float y1, float x2, float y2) {
    return atan2f(y2 - y1, x2 - x1);
}

float Game::normalizeAngle(float a) {
    // Protege contra NaN e infinito (antes um laço infinito travava o jogo)
    if (a != a || a > 1e9f || a < -1e9f) return 0.0f;
    const float PI_F = 3.14159265f;
    a = fmodf(a + PI_F, 2.0f * PI_F);
    if (a < 0) a += 2.0f * PI_F;
    return a - PI_F;
}

Vec2 Game::worldToScreen(float wx, float wy) {
    float sx = (wx - cameraX) * zoom;
    float sy = (wy - cameraY) * zoom;
    return Vec2(sx, sy);
}

bool Game::isInsideWorld(float x, float y) {
    return dist(x, y, 0, 0) < WORLD_RADIUS;
}

Uint32 Game::getColor(Uint8 r, Uint8 g, Uint8 b) {
    return SDL_MapRGB(screen->format, r, g, b);
}

void Game::setPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    if (x < 0 || x >= screen->w || y < 0 || y >= screen->h) return;
    Uint32 *px = (Uint32 *)screen->pixels;
    px[y * (screen->pitch / 4) + x] = getColor(r, g, b);
}