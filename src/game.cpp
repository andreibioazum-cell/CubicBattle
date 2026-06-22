#include "game.h"
#include <GLES2/gl2.h>
#include <cmath>

static const char* VS_SRC = 
    "attribute vec2 a_pos;\n"
    "uniform vec2 u_res;\n"
    "void main() {\n"
    "    vec2 clip = (a_pos / u_res) * 2.0 - 1.0;\n"
    "    clip.y *= -1.0;\n"
    "    gl_Position = vec4(clip, 0.0, 1.0);\n"
    "}\n";

static const char* FS_SRC = 
    "precision mediump float;\n"
    "uniform vec4 u_color;\n"
    "void main() {\n"
    "    gl_FragColor = u_color;\n"
    "}\n";

static GLuint prog = 0;
static GLint aPos, uRes, uColor;

static void initShaders() {
    if (prog != 0) return;
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &VS_SRC, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &FS_SRC, NULL);
    glCompileShader(fs);

    prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    aPos = glGetAttribLocation(prog, "a_pos");
    uRes = glGetUniformLocation(prog, "u_res");
    uColor = glGetUniformLocation(prog, "u_color");
}

Game::Game() {
    playerSize = 50.0f;
    playerSpeed = 600.0f;
    playerPos = {0,0};
    bulletCount = 0;
}

void Game::init(Engine& engine) {
    playerPos = { (float)engine.width / 2.0f, (float)engine.height / 2.0f };
    controls.place(engine.width, engine.height);
    initShaders();
}

void Game::update(float dt, Engine& engine) {
    controls.update(dt);
    Vec2 move = controls.getMove();
    
    playerPos.x += move.x * playerSpeed * dt;
    playerPos.y += move.y * playerSpeed * dt;
    
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

void drawRect(float x, float y, float w, float h, float r, float g, float b, float a, int sw, int sh) {
    GLfloat vertices[] = { x, y, x+w, y, x+w, y+h, x, y, x+w, y+h, x, y+h };
    glUniform4f(uColor, r, g, b, a);
    glUniform2f(uRes, (float)sw, (float)sh);
    glEnableVertexAttribArray(aPos);
    glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(aPos);
}

void Game::draw(Engine& engine) {
    glViewport(0, 0, engine.width, engine.height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(prog);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(int i = 0; i < bulletCount; i++) {
        drawRect(bullets[i].pos.x - 5, bullets[i].pos.y - 5, 10, 10, 1.0f, 0.8f, 0.2f, 1.0f, engine.width, engine.height);
    }

    drawRect(playerPos.x, playerPos.y, playerSize, playerSize, 0.2f, 0.6f, 1.0f, 1.0f, engine.width, engine.height);
    
    drawRect(controls.joy.center.x - controls.joy.radius, controls.joy.center.y - controls.joy.radius, controls.joy.radius*2, controls.joy.radius*2, 0.0f, 0.0f, 0.0f, 0.3f, engine.width, engine.height);
    drawRect(controls.joy.stick.x - controls.joy.stickRadius, controls.joy.stick.y - controls.joy.stickRadius, controls.joy.stickRadius*2, controls.joy.stickRadius*2, 0.0f, 0.0f, 0.0f, 0.8f, engine.width, engine.height);
    drawRect(controls.atkBtn.pos.x - controls.atkBtn.radius, controls.atkBtn.pos.y - controls.atkBtn.radius, controls.atkBtn.radius*2, controls.atkBtn.radius*2, 0.35f, 0.15f, 0.75f, 1.0f, engine.width, engine.height);

    eglSwapBuffers(engine.display, engine.surface);
}
