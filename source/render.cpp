#include "game.h"
#include <stdio.h>
#include <string.h>

// ===== FONTE SIMPLES 5x7 PARA O PS3 (sem precisar de TTF) =====
static unsigned char font5x7(char c) {
    return 0;
}

// Padrão de cada letra - 5 colunas x 7 linhas
static void getCharPixels(char c, unsigned char out[7]) {
    for (int i = 0; i < 7; i++) out[i] = 0;
    switch (c) {
        case 'A': out[0]=0x0E; out[1]=0x11; out[2]=0x11; out[3]=0x1F; out[4]=0x11; out[5]=0x11; out[6]=0x11; break;
        case 'B': out[0]=0x1E; out[1]=0x11; out[2]=0x11; out[3]=0x1E; out[4]=0x11; out[5]=0x11; out[6]=0x1E; break;
        case 'C': out[0]=0x0E; out[1]=0x11; out[2]=0x10; out[3]=0x10; out[4]=0x10; out[5]=0x11; out[6]=0x0E; break;
        case 'D': out[0]=0x1E; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x11; out[5]=0x11; out[6]=0x1E; break;
        case 'E': out[0]=0x1F; out[1]=0x10; out[2]=0x10; out[3]=0x1E; out[4]=0x10; out[5]=0x10; out[6]=0x1F; break;
        case 'F': out[0]=0x1F; out[1]=0x10; out[2]=0x10; out[3]=0x1E; out[4]=0x10; out[5]=0x10; out[6]=0x10; break;
        case 'G': out[0]=0x0E; out[1]=0x11; out[2]=0x10; out[3]=0x17; out[4]=0x11; out[5]=0x11; out[6]=0x0F; break;
        case 'H': out[0]=0x11; out[1]=0x11; out[2]=0x11; out[3]=0x1F; out[4]=0x11; out[5]=0x11; out[6]=0x11; break;
        case 'I': out[0]=0x0E; out[1]=0x04; out[2]=0x04; out[3]=0x04; out[4]=0x04; out[5]=0x04; out[6]=0x0E; break;
        case 'J': out[0]=0x07; out[1]=0x02; out[2]=0x02; out[3]=0x02; out[4]=0x02; out[5]=0x12; out[6]=0x0C; break;
        case 'K': out[0]=0x11; out[1]=0x12; out[2]=0x14; out[3]=0x18; out[4]=0x14; out[5]=0x12; out[6]=0x11; break;
        case 'L': out[0]=0x10; out[1]=0x10; out[2]=0x10; out[3]=0x10; out[4]=0x10; out[5]=0x10; out[6]=0x1F; break;
        case 'M': out[0]=0x11; out[1]=0x1B; out[2]=0x15; out[3]=0x15; out[4]=0x11; out[5]=0x11; out[6]=0x11; break;
        case 'N': out[0]=0x11; out[1]=0x1B; out[2]=0x15; out[3]=0x15; out[4]=0x11; out[5]=0x11; out[6]=0x11; break;
        case 'O': out[0]=0x0E; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x11; out[5]=0x11; out[6]=0x0E; break;
        case 'P': out[0]=0x1E; out[1]=0x11; out[2]=0x11; out[3]=0x1E; out[4]=0x10; out[5]=0x10; out[6]=0x10; break;
        case 'Q': out[0]=0x0E; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x15; out[5]=0x12; out[6]=0x0D; break;
        case 'R': out[0]=0x1E; out[1]=0x11; out[2]=0x11; out[3]=0x1E; out[4]=0x14; out[5]=0x12; out[6]=0x11; break;
        case 'S': out[0]=0x0F; out[1]=0x10; out[2]=0x10; out[3]=0x0E; out[4]=0x01; out[5]=0x01; out[6]=0x1E; break;
        case 'T': out[0]=0x1F; out[1]=0x04; out[2]=0x04; out[3]=0x04; out[4]=0x04; out[5]=0x04; out[6]=0x04; break;
        case 'U': out[0]=0x11; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x11; out[5]=0x11; out[6]=0x0E; break;
        case 'V': out[0]=0x11; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x11; out[5]=0x0A; out[6]=0x04; break;
        case 'X': out[0]=0x11; out[1]=0x0A; out[2]=0x04; out[3]=0x04; out[4]=0x04; out[5]=0x0A; out[6]=0x11; break;
        case 'Y': out[0]=0x11; out[1]=0x0A; out[2]=0x04; out[3]=0x04; out[4]=0x04; out[5]=0x04; out[6]=0x04; break;
        case 'Z': out[0]=0x1F; out[1]=0x01; out[2]=0x02; out[3]=0x04; out[4]=0x08; out[5]=0x10; out[6]=0x1F; break;
        case '0': out[0]=0x0E; out[1]=0x11; out[2]=0x13; out[3]=0x15; out[4]=0x19; out[5]=0x11; out[6]=0x0E; break;
        case '1': out[0]=0x04; out[1]=0x0C; out[2]=0x04; out[3]=0x04; out[4]=0x04; out[5]=0x04; out[6]=0x0E; break;
        case '2': out[0]=0x0E; out[1]=0x11; out[2]=0x01; out[3]=0x02; out[4]=0x04; out[5]=0x08; out[6]=0x1F; break;
        case '3': out[0]=0x1F; out[1]=0x02; out[2]=0x04; out[3]=0x06; out[4]=0x01; out[5]=0x11; out[6]=0x0E; break;
        case '4': out[0]=0x02; out[1]=0x06; out[2]=0x0A; out[3]=0x12; out[4]=0x1F; out[5]=0x02; out[6]=0x02; break;
        case '5': out[0]=0x1F; out[1]=0x10; out[2]=0x1E; out[3]=0x01; out[4]=0x01; out[5]=0x11; out[6]=0x0E; break;
        case '6': out[0]=0x06; out[1]=0x08; out[2]=0x10; out[3]=0x1E; out[4]=0x11; out[5]=0x11; out[6]=0x0E; break;
        case '7': out[0]=0x1F; out[1]=0x01; out[2]=0x02; out[3]=0x04; out[4]=0x08; out[5]=0x08; out[6]=0x08; break;
        case '8': out[0]=0x0E; out[1]=0x11; out[2]=0x11; out[3]=0x0E; out[4]=0x11; out[5]=0x11; out[6]=0x0E; break;
        case '9': out[0]=0x0E; out[1]=0x11; out[2]=0x11; out[3]=0x0F; out[4]=0x01; out[5]=0x02; out[6]=0x0C; break;
        case ' ': out[0]=0; out[1]=0; out[2]=0; out[3]=0; out[4]=0; out[5]=0; out[6]=0; break;
        case '!': out[0]=0x04; out[1]=0x04; out[2]=0x04; out[3]=0x04; out[4]=0x04; out[5]=0x00; out[6]=0x04; break;
        case '.': out[0]=0; out[1]=0; out[2]=0; out[3]=0; out[4]=0; out[5]=0x0C; out[6]=0x0C; break;
        case ':': out[0]=0; out[1]=0x0C; out[2]=0x0C; out[3]=0; out[4]=0x0C; out[5]=0x0C; out[6]=0; break;
        case '-': out[0]=0; out[1]=0; out[2]=0; out[3]=0x1F; out[4]=0; out[5]=0; out[6]=0; break;
        case '>': out[0]=0x08; out[1]=0x04; out[2]=0x02; out[3]=0x01; out[4]=0x02; out[5]=0x04; out[6]=0x08; break;
        default: out[0]=0x1F; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x11; out[5]=0x11; out[6]=0x1F; break;
    }
    (void)font5x7;
}

// ===== RENDER PRINCIPAL =====
void Game::render() {
    if (!screen) return;
    SDL_LockSurface(screen);
    // Fundo preto
    SDL_FillRect(screen, NULL, getColor(12, 12, 20));

    if (gameState == 0) {
        SDL_UnlockSurface(screen);
        renderMenu();
        SDL_Flip(screen);
        return;
    }
    if (gameState == 2) {
        SDL_UnlockSurface(screen);
        renderGameOver();
        SDL_Flip(screen);
        return;
    }

    // Jogo
    drawBackground();
    drawBorder();
    drawFood();
    // Desenha bots primeiro, player por cima
    for (int i = 1; i < MAX_SNAKES; i++) {
        if (snakes[i].alive) drawSnake(snakes[i]);
    }
    if (snakes[0].alive) drawSnake(snakes[0]);
    drawParticles();
    SDL_UnlockSurface(screen);
    drawMinimap();
    drawHUD();
    SDL_Flip(screen);
}

void Game::renderMenu() {
    SDL_FillRect(screen, NULL, getColor(10, 10, 18));
    // Cobra decorativa no menu - círculos coloridos
    for (int i = 0; i < 15; i++) {
        int x = 200 + i * 60;
        int y = 200 + (int)(sinf(i * 0.6f) * 60);
        SDL_LockSurface(screen);
        drawFilledCircle(x, y, 22, 0, 200, 80);
        drawFilledCircle(x - 5, y - 5, 8, 120, 255, 140);
        SDL_UnlockSurface(screen);
    }
    drawText("SLITHER PS3", 430, 100, 0, 255, 120, true);
    drawText("HOMEBREW EDITION", 460, 180, 255, 255, 255, false);
    drawText("PRESSIONE START PARA JOGAR", 320, 420, 255, 255, 0, false);
    drawText("ANALOGICO - MOVER  X / R2 - TURBO", 330, 480, 180, 180, 180, false);
    drawText("COMA PARA CRESCER - NAO BATA NOS OUTROS", 280, 530, 180, 180, 180, false);
}

void Game::renderGameOver() {
    SDL_FillRect(screen, NULL, getColor(20, 5, 5));
    char buf[64];
    snprintf(buf, sizeof(buf), "SCORE: %d", snakes[0].score);
    drawText("VOCE BATEU!", 450, 200, 255, 50, 50, true);
    drawText(buf, 520, 300, 255, 255, 255, false);
    char buf2[64];
    snprintf(buf2, sizeof(buf2), "KILLS: %d", snakes[0].kills);
    drawText(buf2, 520, 350, 255, 255, 0, false);
    drawText("PRESSIONE START PARA JOGAR DE NOVO", 280, 480, 255, 255, 255, false);
}

// ===== FUNDO COM GRADE HEXAGONAL =====
void Game::drawBackground() {
    // Grade que move com a câmera
    int gridSize = (int)(60 * zoom);
    if (gridSize < 20) gridSize = 20;
    int offsetX = (int)(cameraX * zoom) % gridSize;
    int offsetY = (int)(cameraY * zoom) % gridSize;
    Uint32 col = getColor(28, 28, 45);
    Uint32* pixels = (Uint32*)screen->pixels;
    // Linhas verticais e horizontais - hexagonal fake mas bonito
    for (int x = -offsetX; x < SCREEN_W; x += gridSize) {
        if (x < 0 || x >= SCREEN_W) continue;
        for (int y = 0; y < SCREEN_H; y++) {
            pixels[y * SCREEN_W + x] = col;
        }
    }
    for (int y = -offsetY; y < SCREEN_H; y += gridSize) {
        if (y < 0 || y >= SCREEN_H) continue;
        for (int x = 0; x < SCREEN_W; x++) {
            pixels[y * SCREEN_W + x] = col;
        }
    }
    // Pontinhos nos cruzamentos
    for (int x = -offsetX; x < SCREEN_W; x += gridSize) {
        for (int y = -offsetY; y < SCREEN_H; y += gridSize) {
            drawFilledCircle(x, y, 2, 45, 45, 70);
        }
    }
}

void Game::drawBorder() {
    Vec2 c = worldToScreen(0, 0);
    int r = (int)(WORLD_RADIUS * zoom);
    // Borda vermelha luminosa - 3 camadas pra dar glow
    drawCircleOutline((int)c.x, (int)c.y, r + 6, 100, 0, 0);
    drawCircleOutline((int)c.x, (int)c.y, r + 3, 200, 0, 0);
    drawCircleOutline((int)c.x, (int)c.y, r, 255, 40, 40);
    drawCircleOutline((int)c.x, (int)c.y, r - 2, 255, 120, 120);
}

// ===== COMIDA =====
void Game::drawFood() {
    for (int i = 0; i < MAX_FOOD; i++) {
        if (!foods[i].active) continue;
        Vec2 s = worldToScreen(foods[i].pos.x, foods[i].pos.y);
        if (s.x < -30 || s.x > SCREEN_W + 30 || s.y < -30 || s.y > SCREEN_H + 30) continue;
        float pulse = 1.0f + sinf(foods[i].pulse) * 0.25f;
        int rad = (int)(foods[i].radius * pulse * zoom);
        if (rad < 2) rad = 2;
        // Glow
        drawGlowCircle((int)s.x, (int)s.y, rad + 6, foods[i].r, foods[i].g, foods[i].b, 80);
        drawFilledCircle((int)s.x, (int)s.y, rad, foods[i].r, foods[i].g, foods[i].b);
        // Brilho branco no meio
        drawFilledCircle((int)s.x - rad/3, (int)s.y - rad/3, rad/3, 255, 255, 255);
    }
}

// ===== COBRA COM EFEITO 3D =====
void Game::drawSnake(Snake &s) {
    if (!s.alive) return;
    // Desenha do rabo pra cabeça
    for (int i = (int)s.length - 1; i >= 0; i--) {
        if (i >= (int)s.body.size()) continue;
        Vec2 p = s.body[i];
        Vec2 scr = worldToScreen(p.x, p.y);
        if (scr.x < -80 || scr.x > SCREEN_W + 80 || scr.y < -80 || scr.y > SCREEN_H + 80) continue;
        int rad = (int)(s.radius * zoom);
        if (rad < 4) rad = 4;
        // Listrado: alterna cor principal e secundaria a cada 3 segmentos
        bool stripe = (i / 3) % 2 == 0;
        Uint8 r = stripe ? s.colorR : s.colorR2;
        Uint8 g = stripe ? s.colorG : s.colorG2;
        Uint8 b = stripe ? s.colorB : s.colorB2;
        // Se é turbo, brilha mais
        if (s.boosting) {
            drawGlowCircle((int)scr.x, (int)scr.y, rad + 12, r, g, b, 120);
        }
        // Sombra / borda escura
        drawFilledCircle((int)scr.x, (int)scr.y, rad + 1, 0, 0, 0);
        // Corpo principal
        drawFilledCircle((int)scr.x, (int)scr.y, rad, r, g, b);
        // Brilho 3D em cima
        drawFilledCircle((int)scr.x - rad/3, (int)scr.y - rad/3, rad/2, (Uint8)(r + 60 > 255 ? 255 : r + 60), (Uint8)(g + 60 > 255 ? 255 : g + 60), (Uint8)(b + 60 > 255 ? 255 : b + 60));
        // Ponto escuro embaixo pra volume
        drawFilledCircle((int)scr.x + rad/4, (int)scr.y + rad/4, rad/3, (Uint8)(r * 0.5f), (Uint8)(g * 0.5f), (Uint8)(b * 0.5f));
    }

    // OLHOS na cabeça
    Vec2 head = s.body[0];
    Vec2 scr = worldToScreen(head.x, head.y);
    int rad = (int)(s.radius * zoom);
    float ex = cosf(s.angle);
    float ey = sinf(s.angle);
    float px = -ey;
    float py = ex;
    // Olho esquerdo e direito
    int eyeOff = rad / 2;
    int eyeForward = rad / 2;
    int lx = (int)(scr.x + ex * eyeForward + px * eyeOff);
    int ly = (int)(scr.y + ey * eyeForward + py * eyeOff);
    int rx = (int)(scr.x + ex * eyeForward - px * eyeOff);
    int ry = (int)(scr.y + ey * eyeForward - py * eyeOff);
    int eyeR = rad / 2;
    if (eyeR < 4) eyeR = 4;
    // Branco do olho
    drawFilledCircle(lx, ly, eyeR, 255, 255, 255);
    drawFilledCircle(rx, ry, eyeR, 255, 255, 255);
    // Pupila olhando pra direção
    int pupilR = eyeR / 2;
    if (pupilR < 2) pupilR = 2;
    int pupOff = eyeR / 3;
    drawFilledCircle(lx + (int)(ex * pupOff), ly + (int)(ey * pupOff), pupilR, 0, 0, 0);
    drawFilledCircle(rx + (int)(ex * pupOff), ry + (int)(ey * pupOff), pupilR, 0, 0, 0);
    // Brilhinho no olho
    drawFilledCircle(lx + (int)(ex * pupOff) - 1, ly + (int)(ey * pupOff) - 1, 1, 255, 255, 255);
    drawFilledCircle(rx + (int)(ex * pupOff) - 1, ry + (int)(ey * pupOff) - 1, 1, 255, 255, 255);
}

void Game::drawParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        Vec2 s = worldToScreen(particles[i].pos.x, particles[i].pos.y);
        if (s.x < 0 || s.x >= SCREEN_W || s.y < 0 || s.y >= SCREEN_H) continue;
        float a = particles[i].life / particles[i].maxLife;
        int rad = (int)(particles[i].size * a * zoom);
        if (rad < 1) rad = 1;
        drawGlowCircle((int)s.x, (int)s.y, rad + 4, particles[i].r, particles[i].g, particles[i].b, (Uint8)(150 * a));
        drawFilledCircle((int)s.x, (int)s.y, rad, particles[i].r, particles[i].g, particles[i].b);
    }
}

// ===== MINIMAPA CIRCULAR =====
void Game::drawMinimap() {
    int cx = SCREEN_W - 110;
    int cy = SCREEN_H - 110;
    int r = 80;
    // Fundo do minimapa
    SDL_LockSurface(screen);
    drawFilledCircle(cx, cy, r + 2, 0, 0, 0);
    drawFilledCircle(cx, cy, r, 20, 20, 35);
    drawCircleOutline(cx, cy, r, 255, 255, 255);
    // Borda do mundo no minimapa
    drawCircleOutline(cx, cy, r - 2, 255, 50, 50);
    // Bots como pontinhos
    for (int i = 1; i < MAX_SNAKES; i++) {
        if (!snakes[i].alive) continue;
        int mx = cx + (int)(snakes[i].body[0].x / WORLD_RADIUS * r);
        int my = cy + (int)(snakes[i].body[0].y / WORLD_RADIUS * r);
        drawFilledCircle(mx, my, 3, snakes[i].colorR, snakes[i].colorG, snakes[i].colorB);
    }
    // Player branco maior
    if (snakes[0].alive) {
        int mx = cx + (int)(snakes[0].body[0].x / WORLD_RADIUS * r);
        int my = cy + (int)(snakes[0].body[0].y / WORLD_RADIUS * r);
        drawFilledCircle(mx, my, 5, 255, 255, 255);
        drawFilledCircle(mx, my, 3, 0, 255, 120);
    }
    SDL_UnlockSurface(screen);
}

void Game::drawHUD() {
    // Placar no canto superior direito
    char buf[64];
    snprintf(buf, sizeof(buf), "SCORE: %d", snakes[0].score);
    drawText(buf, 1050, 20, 255, 255, 255, false);
    snprintf(buf, sizeof(buf), "SIZE: %d", (int)snakes[0].length);
    drawText(buf, 1050, 50, 0, 255, 120, false);
    snprintf(buf, sizeof(buf), "KILLS: %d", snakes[0].kills);
    drawText(buf, 1050, 80, 255, 200, 0, false);

    // Leaderboard simples - top 5
    drawText("TOP 5", 1050, 130, 255, 255, 0, false);
    // Ordena cópia simples por score
    int order[MAX_SNAKES];
    for (int i = 0; i < MAX_SNAKES; i++) order[i] = i;
    for (int i = 0; i < MAX_SNAKES; i++) {
        for (int j = i + 1; j < MAX_SNAKES; j++) {
            if (snakes[order[j]].score > snakes[order[i]].score) {
                int t = order[i]; order[i] = order[j]; order[j] = t;
            }
        }
    }
    for (int k = 0; k < 5; k++) {
        int idx = order[k];
        if (!snakes[idx].alive && idx != 0) continue;
        char line[64];
        if (idx == 0) snprintf(line, sizeof(line), "%d. VOCE %d", k + 1, snakes[idx].score);
        else snprintf(line, sizeof(line), "%d. BOT %d", k + 1, snakes[idx].score);
        Uint8 rr = idx == 0 ? 0 : 200;
        Uint8 gg = idx == 0 ? 255 : 200;
        Uint8 bb = idx == 0 ? 120 : 200;
        drawText(line, 1050, 160 + k * 30, rr, gg, bb, false);
    }

    // Barra de turbo embaixo
    if (snakes[0].boosting) {
        drawText("TURBO!", 30, SCREEN_H - 40, 255, 255, 0, false);
    }
    // Nome no canto inferior esquerdo
    drawText("SLITHER PS3 - HOMEBREW", 20, 20, 180, 180, 180, false);
}

// ===== PRIMITIVAS DE DESENHO =====
void Game::drawGlowCircle(int sx, int sy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha) {
    // Glow simples: desenha vários círculos transparentes simulados com cor mais escura
    for (int i = radius; i > 0; i -= 2) {
        Uint8 f = (Uint8)((i * alpha) / (radius + 1));
        // Mistura com fundo escuro pra simular transparência
        Uint8 mr = (Uint8)((r * f + 12 * (255 - f)) / 255);
        Uint8 mg = (Uint8)((g * f + 12 * (255 - f)) / 255);
        Uint8 mb = (Uint8)((b * f + 20 * (255 - f)) / 255);
        drawFilledCircle(sx, sy, i, mr, mg, mb);
    }
}

void Game::drawFilledCircle(int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b) {
    if (radius <= 0) return;
    Uint32 col = getColor(r, g, b);
    Uint32* pixels = (Uint32*)screen->pixels;
    for (int y = -radius; y <= radius; y++) {
        int yy = cy + y;
        if (yy < 0 || yy >= SCREEN_H) continue;
        int dx = (int)sqrtf((float)(radius * radius - y * y));
        for (int x = -dx; x <= dx; x++) {
            int xx = cx + x;
            if (xx < 0 || xx >= SCREEN_W) continue;
            pixels[yy * SCREEN_W + xx] = col;
        }
    }
}

void Game::drawCircleOutline(int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b) {
    if (radius <= 0) return;
    Uint32 col = getColor(r, g, b);
    Uint32* pixels = (Uint32*)screen->pixels;
    for (int a = 0; a < 360; a++) {
        float rad = a * 0.0174533f;
        int x = cx + (int)(cosf(rad) * radius);
        int y = cy + (int)(sinf(rad) * radius);
        if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H) continue;
        pixels[y * SCREEN_W + x] = col;
        // Espessura 2
        if (x + 1 < SCREEN_W) pixels[y * SCREEN_W + x + 1] = col;
    }
}

void Game::drawText(const char* text, int x, int y, Uint8 r, Uint8 g, Uint8 b, bool big) {
    if (!text) return;
    SDL_LockSurface(screen);
    int scale = big ? 4 : 2;
    int cx = x;
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        char c = text[i];
        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
        unsigned char rows[7];
        getCharPixels(c, rows);
        for (int row = 0; row < 7; row++) {
            for (int colBit = 0; colBit < 5; colBit++) {
                int bit = (rows[row] >> (4 - colBit)) & 1;
                if (bit) {
                    // Desenha bloco scale x scale
                    for (int dy = 0; dy < scale; dy++) {
                        for (int dx = 0; dx < scale; dx++) {
                            int px = cx + colBit * scale + dx;
                            int py = y + row * scale + dy;
                            if (px >= 0 && px < SCREEN_W && py >= 0 && py < SCREEN_H) {
                                Uint32* pixels = (Uint32*)screen->pixels;
                                pixels[py * SCREEN_W + px] = getColor(r, g, b);
                            }
                        }
                    }
                }
            }
        }
        cx += 6 * scale + 2;
    }
    SDL_UnlockSurface(screen);
}