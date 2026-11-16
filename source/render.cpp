#include "game.h"
#include <stdio.h>
#include <string.h>

// Tamanho real da tela (lido do SDL, não da constante)
#define SW (screen->w)
#define SH (screen->h)

static int clampi(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// Largura em pixels de um texto desenhado com drawText
static int textWidth(const char *t, bool big) {
    int scale = big ? 4 : 2;
    return (int)strlen(t) * (6 * scale + 2);
}

// ===== FONTE 5x7 =====
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
        case 'N': out[0]=0x11; out[1]=0x19; out[2]=0x15; out[3]=0x13; out[4]=0x11; out[5]=0x11; out[6]=0x11; break; // corrigido
        case 'O': out[0]=0x0E; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x11; out[5]=0x11; out[6]=0x0E; break;
        case 'P': out[0]=0x1E; out[1]=0x11; out[2]=0x11; out[3]=0x1E; out[4]=0x10; out[5]=0x10; out[6]=0x10; break;
        case 'Q': out[0]=0x0E; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x15; out[5]=0x12; out[6]=0x0D; break;
        case 'R': out[0]=0x1E; out[1]=0x11; out[2]=0x11; out[3]=0x1E; out[4]=0x14; out[5]=0x12; out[6]=0x11; break;
        case 'S': out[0]=0x0F; out[1]=0x10; out[2]=0x10; out[3]=0x0E; out[4]=0x01; out[5]=0x01; out[6]=0x1E; break;
        case 'T': out[0]=0x1F; out[1]=0x04; out[2]=0x04; out[3]=0x04; out[4]=0x04; out[5]=0x04; out[6]=0x04; break;
        case 'U': out[0]=0x11; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x11; out[5]=0x11; out[6]=0x0E; break;
        case 'V': out[0]=0x11; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x11; out[5]=0x0A; out[6]=0x04; break;
        case 'W': out[0]=0x11; out[1]=0x11; out[2]=0x11; out[3]=0x15; out[4]=0x15; out[5]=0x15; out[6]=0x0A; break; // novo
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
        case ' ': break;
        case '!': out[0]=0x04; out[1]=0x04; out[2]=0x04; out[3]=0x04; out[4]=0x04; out[5]=0x00; out[6]=0x04; break;
        case '.': out[5]=0x0C; out[6]=0x0C; break;
        case ':': out[1]=0x0C; out[2]=0x0C; out[4]=0x0C; out[5]=0x0C; break;
        case '-': out[3]=0x1F; break;
        case '>': out[0]=0x08; out[1]=0x04; out[2]=0x02; out[3]=0x01; out[4]=0x02; out[5]=0x04; out[6]=0x08; break;
        default:  out[0]=0x1F; out[1]=0x11; out[2]=0x11; out[3]=0x11; out[4]=0x11; out[5]=0x11; out[6]=0x1F; break;
    }
}

// ===== RENDER PRINCIPAL =====
// Uma única chamada de lock por frame. As funções de desenho abaixo
// NÃO fazem lock: assumem que a tela já está travada.
void Game::render() {
    if (!screen) return;

    if (gameState == 2) SDL_FillRect(screen, NULL, getColor(20, 5, 5));
    else                SDL_FillRect(screen, NULL, getColor(12, 12, 20));

    SDL_LockSurface(screen);
    if (gameState == 0) {
        renderMenu();
    } else if (gameState == 2) {
        renderGameOver();
    } else {
        drawBackground();
        drawBorder();
        drawFood();
        for (int i = 1; i < MAX_SNAKES; i++) {
            if (snakes[i].alive) drawSnake(snakes[i]);
        }
        if (snakes[0].alive) drawSnake(snakes[0]);
        drawParticles();
        drawMinimap();
        drawHUD();
    }
    SDL_UnlockSurface(screen);
    SDL_Flip(screen);
}

void Game::renderMenu() {
    // Cobra decorativa no menu
    for (int i = 0; i < 15; i++) {
        int x = SW / 2 - 420 + i * 60;
        int y = SH / 3 + (int)(sinf(i * 0.6f) * 60);
        drawFilledCircle(x, y, 22, 0, 200, 80);
        drawFilledCircle(x - 5, y - 5, 8, 120, 255, 140);
    }
    const char *t1 = "SLITHER PS3";
    const char *t2 = "HOMEBREW EDITION";
    const char *t3 = "PRESSIONE START PARA JOGAR";
    const char *t4 = "ANALOGICO - MOVER  X / R2 - TURBO";
    const char *t5 = "COMA PARA CRESCER - NAO BATA NOS OUTROS";
    int y0 = SH / 2 - 60;
    drawText(t1, (SW - textWidth(t1, true)) / 2, y0, 0, 255, 120, true);
    drawText(t2, (SW - textWidth(t2, false)) / 2, y0 + 80, 255, 255, 255, false);
    drawText(t3, (SW - textWidth(t3, false)) / 2, y0 + 200, 255, 255, 0, false);
    drawText(t4, (SW - textWidth(t4, false)) / 2, y0 + 260, 180, 180, 180, false);
    drawText(t5, (SW - textWidth(t5, false)) / 2, y0 + 310, 180, 180, 180, false);
}

void Game::renderGameOver() {
    char buf[64];
    char buf2[64];
    snprintf(buf, sizeof(buf), "SCORE: %d", snakes[0].score);
    snprintf(buf2, sizeof(buf2), "KILLS: %d", snakes[0].kills);
    const char *t1 = "VOCE BATEU!";
    const char *t3 = "PRESSIONE START PARA JOGAR DE NOVO";
    int y0 = SH / 3;
    drawText(t1, (SW - textWidth(t1, true)) / 2, y0, 255, 50, 50, true);
    drawText(buf, (SW - textWidth(buf, false)) / 2, y0 + 100, 255, 255, 255, false);
    drawText(buf2, (SW - textWidth(buf2, false)) / 2, y0 + 150, 255, 255, 0, false);
    drawText(t3, (SW - textWidth(t3, false)) / 2, y0 + 280, 255, 255, 255, false);
}

// ===== FUNDO COM GRADE =====
void Game::drawBackground() {
    int gridSize = (int)(60 * zoom);
    if (gridSize < 20) gridSize = 20;
    int offX = (int)(cameraX * zoom) % gridSize;
    int offY = (int)(cameraY * zoom) % gridSize;
    if (offX < 0) offX += gridSize;
    if (offY < 0) offY += gridSize;

    Uint32 col = getColor(28, 28, 45);
    Uint32 dot = getColor(45, 45, 70);
    Uint32 *px = (Uint32 *)screen->pixels;
    int st = screen->pitch / 4;

    for (int x = offX; x < SW; x += gridSize)
        for (int y = 0; y < SH; y++)
            px[y * st + x] = col;

    for (int y = offY; y < SH; y += gridSize)
        for (int x = 0; x < SW; x++)
            px[y * st + x] = col;

    for (int x = offX; x < SW; x += gridSize)
        for (int y = offY; y < SH; y += gridSize)
            px[y * st + x] = dot;
}

void Game::drawBorder() {
    Vec2 c = worldToScreen(0, 0);
    int r = (int)(WORLD_RADIUS * zoom);
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
        if (s.x < -30 || s.x > SW + 30 || s.y < -30 || s.y > SH + 30) continue;
        float pulse = 1.0f + sinf(foods[i].pulse) * 0.25f;
        int rad = (int)(foods[i].radius * pulse * zoom);
        if (rad < 2) rad = 2;
        drawGlowCircle((int)s.x, (int)s.y, rad + 4, foods[i].r, foods[i].g, foods[i].b, 80);
        drawFilledCircle((int)s.x, (int)s.y, rad, foods[i].r, foods[i].g, foods[i].b);
        drawFilledCircle((int)s.x - rad / 3, (int)s.y - rad / 3, rad / 3 + 1, 255, 255, 255);
    }
}

// ===== COBRA =====
void Game::drawSnake(Snake &s) {
    if (!s.alive) return;
    int n = (int)s.body.size();
    int segs = (int)s.length;
    if (segs > n) segs = n;

    for (int i = segs - 1; i >= 0; i--) {
        Vec2 scr = worldToScreen(s.body[i].x, s.body[i].y);
        if (scr.x < -80 || scr.x > SW + 80 || scr.y < -80 || scr.y > SH + 80) continue;
        int sx = (int)scr.x;
        int sy = (int)scr.y;
        int rad = (int)(s.radius * zoom);
        if (rad < 4) rad = 4;

        bool stripe = (i / 3) % 2 == 0;
        Uint8 r = stripe ? s.colorR : s.colorR2;
        Uint8 g = stripe ? s.colorG : s.colorG2;
        Uint8 b = stripe ? s.colorB : s.colorB2;

        // Glow do turbo só na cabeça (antes era em todo segmento e pesava)
        if (s.boosting && i == 0) {
            drawGlowCircle(sx, sy, rad + 8, r, g, b, 120);
        }
        drawFilledCircle(sx, sy, rad + 1, 0, 0, 0);
        drawFilledCircle(sx, sy, rad, r, g, b);
        drawFilledCircle(sx - rad / 3, sy - rad / 3, rad / 2,
                         (Uint8)(r + 60 > 255 ? 255 : r + 60),
                         (Uint8)(g + 60 > 255 ? 255 : g + 60),
                         (Uint8)(b + 60 > 255 ? 255 : b + 60));
        drawFilledCircle(sx + rad / 4, sy + rad / 4, rad / 3,
                         (Uint8)(r * 0.5f), (Uint8)(g * 0.5f), (Uint8)(b * 0.5f));
    }

    // Olhos na cabeça
    if (n == 0) return;
    Vec2 scr = worldToScreen(s.body[0].x, s.body[0].y);
    int rad = (int)(s.radius * zoom);
    float ex = cosf(s.angle);
    float ey = sinf(s.angle);
    float px = -ey;
    float py = ex;
    int eyeOff = rad / 2;
    int eyeForward = rad / 2;
    int lx = (int)(scr.x + ex * eyeForward + px * eyeOff);
    int ly = (int)(scr.y + ey * eyeForward + py * eyeOff);
    int rx = (int)(scr.x + ex * eyeForward - px * eyeOff);
    int ry = (int)(scr.y + ey * eyeForward - py * eyeOff);
    int eyeR = rad / 2;
    if (eyeR < 4) eyeR = 4;
    drawFilledCircle(lx, ly, eyeR, 255, 255, 255);
    drawFilledCircle(rx, ry, eyeR, 255, 255, 255);
    int pupilR = eyeR / 2;
    if (pupilR < 2) pupilR = 2;
    int pupOff = eyeR / 3;
    drawFilledCircle(lx + (int)(ex * pupOff), ly + (int)(ey * pupOff), pupilR, 0, 0, 0);
    drawFilledCircle(rx + (int)(ex * pupOff), ry + (int)(ey * pupOff), pupilR, 0, 0, 0);
}

void Game::drawParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        Vec2 s = worldToScreen(particles[i].pos.x, particles[i].pos.y);
        if (s.x < 0 || s.x >= SW || s.y < 0 || s.y >= SH) continue;
        float a = particles[i].life / particles[i].maxLife;
        int rad = (int)(particles[i].size * a * zoom);
        if (rad < 1) rad = 1;
        drawFilledCircle((int)s.x, (int)s.y, rad, particles[i].r, particles[i].g, particles[i].b);
    }
}

// ===== MINIMAPA CIRCULAR =====
void Game::drawMinimap() {
    int r = 80;
    int cx = SW - 110;
    int cy = SH - 110;
    drawFilledCircle(cx, cy, r + 2, 0, 0, 0);
    drawFilledCircle(cx, cy, r, 20, 20, 35);
    drawCircleOutline(cx, cy, r, 255, 255, 255);

    for (int i = 1; i < MAX_SNAKES; i++) {
        if (!snakes[i].alive) continue;
        int mx = clampi(cx + (int)(snakes[i].body[0].x / WORLD_RADIUS * r), cx - r, cx + r);
        int my = clampi(cy + (int)(snakes[i].body[0].y / WORLD_RADIUS * r), cy - r, cy + r);
        drawFilledCircle(mx, my, 3, snakes[i].colorR, snakes[i].colorG, snakes[i].colorB);
    }
    if (snakes[0].alive) {
        int mx = clampi(cx + (int)(snakes[0].body[0].x / WORLD_RADIUS * r), cx - r, cx + r);
        int my = clampi(cy + (int)(snakes[0].body[0].y / WORLD_RADIUS * r), cy - r, cy + r);
        drawFilledCircle(mx, my, 5, 255, 255, 255);
        drawFilledCircle(mx, my, 3, 0, 255, 120);
    }
}

void Game::drawHUD() {
    int hx = SW - 230;
    char buf[64];
    snprintf(buf, sizeof(buf), "SCORE: %d", snakes[0].score);
    drawText(buf, hx, 20, 255, 255, 255, false);
    snprintf(buf, sizeof(buf), "SIZE: %d", (int)snakes[0].length);
    drawText(buf, hx, 50, 0, 255, 120, false);
    snprintf(buf, sizeof(buf), "KILLS: %d", snakes[0].kills);
    drawText(buf, hx, 80, 255, 200, 0, false);

    drawText("TOP 5", hx, 130, 255, 255, 0, false);
    int order[MAX_SNAKES];
    for (int i = 0; i < MAX_SNAKES; i++) order[i] = i;
    for (int i = 0; i < MAX_SNAKES; i++) {
        for (int j = i + 1; j < MAX_SNAKES; j++) {
            if (snakes[order[j]].score > snakes[order[i]].score) {
                int t = order[i]; order[i] = order[j]; order[j] = t;
            }
        }
    }
    int shown = 0;
    for (int k = 0; k < MAX_SNAKES && shown < 5; k++) {
        int idx = order[k];
        if (!snakes[idx].alive && idx != 0) continue;
        char line[64];
        if (idx == 0) snprintf(line, sizeof(line), "%d. VOCE %d", shown + 1, snakes[idx].score);
        else          snprintf(line, sizeof(line), "%d. BOT %d", shown + 1, snakes[idx].score);
        Uint8 rr = idx == 0 ? 0 : 200;
        Uint8 gg = idx == 0 ? 255 : 200;
        Uint8 bb = idx == 0 ? 120 : 200;
        drawText(line, hx, 160 + shown * 30, rr, gg, bb, false);
        shown++;
    }

    if (snakes[0].boosting) {
        drawText("TURBO!", 30, SH - 40, 255, 255, 0, false);
    }
    drawText("SLITHER PS3 - HOMEBREW", 20, 20, 180, 180, 180, false);
}

// ===== PRIMITIVAS DE DESENHO (sem lock) =====
void Game::drawGlowCircle(int sx, int sy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha) {
    // Um único círculo misturado com o fundo escuro (antes eram dezenas, e pesava)
    int a = alpha;
    Uint8 mr = (Uint8)((r * a + 12 * (255 - a)) / 255);
    Uint8 mg = (Uint8)((g * a + 12 * (255 - a)) / 255);
    Uint8 mb = (Uint8)((b * a + 20 * (255 - a)) / 255);
    drawFilledCircle(sx, sy, radius, mr, mg, mb);
}

void Game::drawFilledCircle(int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b) {
    if (radius <= 0) return;
    Uint32 col = getColor(r, g, b);
    Uint32 *px = (Uint32 *)screen->pixels;
    int st = screen->pitch / 4;
    for (int y = -radius; y <= radius; y++) {
        int yy = cy + y;
        if (yy < 0 || yy >= SH) continue;
        int dx = (int)sqrtf((float)(radius * radius - y * y));
        int x0 = clampi(cx - dx, 0, SW - 1);
        int x1 = clampi(cx + dx, 0, SW - 1);
        for (int xx = x0; xx <= x1; xx++) {
            px[yy * st + xx] = col;
        }
    }
}

void Game::drawCircleOutline(int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b) {
    if (radius <= 0) return;
    Uint32 col = getColor(r, g, b);
    Uint32 *px = (Uint32 *)screen->pixels;
    int st = screen->pitch / 4;
    for (int a = 0; a < 360; a++) {
        float rad = a * 0.0174533f;
        int x = cx + (int)(cosf(rad) * radius);
        int y = cy + (int)(sinf(rad) * radius);
        if (x < 0 || x + 1 >= SW || y < 0 || y >= SH) continue;
        px[y * st + x] = col;
        px[y * st + x + 1] = col; // espessura 2
    }
}

void Game::drawText(const char* text, int x, int y, Uint8 r, Uint8 g, Uint8 b, bool big) {
    if (!text) return;
    int scale = big ? 4 : 2;
    Uint32 col = getColor(r, g, b);
    Uint32 *px = (Uint32 *)screen->pixels;
    int st = screen->pitch / 4;
    int cx = x;
    int len = (int)strlen(text);
    for (int i = 0; i < len; i++) {
        char c = text[i];
        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
        unsigned char rows[7];
        getCharPixels(c, rows);
        for (int row = 0; row < 7; row++) {
            for (int colBit = 0; colBit < 5; colBit++) {
                if (!((rows[row] >> (4 - colBit)) & 1)) continue;
                for (int dy = 0; dy < scale; dy++) {
                    for (int dx = 0; dx < scale; dx++) {
                        int X = cx + colBit * scale + dx;
                        int Y = y + row * scale + dy;
                        if (X >= 0 && X < SW && Y >= 0 && Y < SH) {
                            px[Y * st + X] = col;
                        }
                    }
                }
            }
        }
        cx += 6 * scale + 2;
    }
}