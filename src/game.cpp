#include "game.h"
#include <GLES2/gl2.h>
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

void Game::update(float dt) {
    controls.update(dt);
    Vec2 move = controls.getMove();
    
    playerPos.x += move.x * playerSpeed * dt;
    playerPos.y += move.y * playerSpeed * dt;
    
    // Ограничения экрана
    playerPos.x = std::max(0.0f, std::min((float)engine.width - playerSize, playerPos.x)); // упрощено
    playerPos.y = std::max(0.0f, std::min((float)engine.height - playerSize, playerPos.y));
    
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

// Простейшая отрисовка прямоугольников (без VBO, для примера)
void drawRect(float x, float y, float w, float h, float r, float g, float b) {
    GLfloat vertices[] = {
        x, y,     r, g, b,
        x+w, y,   r, g, b,
        x+w, y+h, r, g, b,
        x, y,     r, g, b,
        x+w, y+h, r, g, b,
        x, y+h,   r, g, b
    };
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &vertices[2]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Game::draw(Engine& engine) {
    // Очистка фона
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(0); // Фиксированный пайплайн (устарело, но работает в GLES 2.0 для простоты)
    
    // Ортографическая проекция (на лету)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, engine.width, engine.height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Игрок
    drawRect(playerPos.x, playerPos.y, playerSize, playerSize, 0.2f, 0.6f, 1.0f);
    
    // Пули
    for(int i=0; i<bulletCount; i++) {
        drawRect(bullets[i].pos.x - 5, bullets[i].pos.y - 5, 10, 10, 1.0f, 0.8f, 0.2f);
    }
    
    // UI (Джойстик)
    // (Рисуем упрощенно)
    glColor4f(0, 0, 0, 0.2f); drawRect(controls.joy.center.x - controls.joy.radius, controls.joy.center.y - controls.joy.radius, controls.joy.radius*2, controls.joy.radius*2, 0,0,0);
    
    eglSwapBuffers(engine.display, engine.surface);
}
