#pragma once
#include "math_utils.h"
#include "controls.h"
#include "engine.h"

struct Bullet {
    Vec2 pos;
    Vec2 dir;
    float life;
};

class Game {
public:
    Controls controls;
    Vec2 playerPos;
    float playerSize;
    float playerSpeed;
    
    static const int MAX_BULLETS = 100;
    Bullet bullets[MAX_BULLETS];
    int bulletCount;

    Game();
    void init(Engine& engine);
    void update(float dt);
    void draw(Engine& engine);
};
