#include "game.h"
#include <GLES/gl.h> // Используем OpenGL ES 1.x
#include <cmath>

Game::Game() {
    playerSize = 50.0f;
    playerSpeed = 600.0f;
    playerPos = {0,0};
    bulletCount = 0;
}

void Game::init(Engine& engine) {
    playerPos = { (float)engine.width / 2.0f, (float)engine.height / 2.0f };
    controls.place(engine.width, engine.height);
}

void Game::update(float dt, Engine& engine) {
    controls.update(dt);
    Vec2 move = controls.getMove();
    
    playerPos.x += move.x * playerSpeed * dt;
    playerPos.y += move.y * playerSpeed * dt;
    
    // Экран
    if (playerPos.x < 0) playerPos.x = 0;
    if (playerPos.y < 0) playerPos.y = 0;
    if (playerPos.x > engine.width - playerSize) playerPos.x = engine.width - playerSize;
    if (playerPos.y > engine.height - playerSize) playerPos.y = engine.height - playerSize;
    
    Vec2 aim;
    if (controls.getShot(aim) && bulletCount < MAX_BULLETS) {
        bullets[bulletCount++] = { 
            { playerPos.x + playerSize/2, playerPos.y + playerSize/2 }, 
            aim, 
            2.0f 
        };
    }
    
    for (int i = 0; i < bulletCount; i++) {
        bullets[i].pos.x += bullets[i].dir.x * 800.0f * dt;
        bullets[i].pos.y += bullets[i].dir.y * 800.0f * dt;
        bullets[i].life -= dt;
        if (bullets[i].life <= 0) {
            bullets[i] = bullets[bulletCount - 1];
            bulletCount--;
            i--;
        }
    }
}

// Простая отрисовка прямоугольника в OpenGL ES 1.x
void drawRect(float x, float y, float w, float h, float r, float g, float b, float a = 1.0f) {
    GLfloat vertices[] = {
        x, y,     x+w, y,   x+w, y+h,
        x, y,     x+w, y+h, x, y+h
    };
    glColor4f(r, g, b, a);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Game::draw(Engine& engine) {
    // Настройка 2D камеры
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, engine.width, engine.height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Фон
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Пули (желтые)
    for(int i = 0; i < bulletCount; i++) {
        drawRect(bullets[i].pos.x - 5, bullets[i].pos.y - 5, 10, 10, 1.0f, 0.8f, 0.2f);
    }

    // Игрок (синий)
    drawRect(playerPos.x, playerPos.y, playerSize, playerSize, 0.2f, 0.6f, 1.0f);
    
    // Джойстик (темный полупрозрачный)
    drawRect(controls.joy.center.x - controls.joy.radius, 
             controls.joy.center.y - controls.joy.radius, 
             controls.joy.radius*2, controls.joy.radius*2, 
             0.0f, 0.0f, 0.0f, 0.3f);
             
    // Стик джойстика
    drawRect(controls.joy.stick.x - controls.joy.stickRadius, 
             controls.joy.stick.y - controls.joy.stickRadius, 
             controls.joy.stickRadius*2, controls.joy.stickRadius*2, 
             0.0f, 0.0f, 0.0f, 0.8f);

    // Кнопка атаки (фиолетовая)
    drawRect(controls.atkBtn.pos.x - controls.atkBtn.radius, 
             controls.atkBtn.pos.y - controls.atkBtn.radius, 
             controls.atkBtn.radius*2, controls.atkBtn.radius*2, 
             0.35f, 0.15f, 0.75f, 1.0f);

    eglSwapBuffers(engine.display, engine.surface);
}
