#include "controls.h"
#include <cmath>

Controls::Controls() {
    joy = { -1, {0,0}, {0,0}, 90.0f, 36.0f };
    atkBtn = { -1, {0,0}, 104.0f, false, 0.0f };
    backBtn = { -1, {0,0}, 80.0f, false, 0.0f }; // Упрощенная кнопка Back (круг)
    keys = { false, false, false, false, false };
    aimDir = {0, -1};
    spaceJustPressed = false;
    isMobile = false;
}

void Controls::place(int w, int h) {
    joy.center = { 160.0f, (float)h - 160.0f };
    if (joy.id == -1) joy.stick = joy.center;
    atkBtn.pos = { (float)w - 160.0f, (float)h - 160.0f };
    backBtn.pos = { (float)w / 2.0f, 80.0f };
}

void Controls::update(float dt) {
    float target = atkBtn.hold ? 1.0f : 0.0f;
    atkBtn.pressAnim += (target - atkBtn.pressAnim) * std::min(dt * 12.0f, 1.0f);
}

Vec2 Controls::getMove() {
    Vec2 dx = {0, 0};
    if (keys.w) dx.y -= 1;
    if (keys.s) dx.y += 1;
    if (keys.a) dx.x -= 1;
    if (keys.d) dx.x += 1;
    
    if (joy.id != -1) {
        Vec2 jdx = { joy.stick.x - joy.center.x, joy.stick.y - joy.center.y };
        if (jdx.length() > 0) {
            if (dx.x == 0 && dx.y == 0) {
                dx = jdx.normalize();
                aimDir = dx;
            }
        }
    }
    
    if (dx.x != 0 || dx.y != 0) {
        dx = dx.normalize();
        aimDir = dx;
    }
    return dx;
}

bool Controls::getShot(Vec2& outAim) {
    outAim = aimDir;
    if (spaceJustPressed) {
        spaceJustPressed = false;
        return true;
    }
    return false;
}

void Controls::touchPressed(int id, float x, float y) {
    // Back button
    Vec2 bDist = { x - backBtn.pos.x, y - backBtn.pos.y };
    if (bDist.length() <= backBtn.radius) {
        // В C++ нет GameState, тут нужно будет вызывать коллбек
        return;
    }
    
    Vec2 jDist = { x - joy.center.x, y - joy.center.y };
    if (jDist.length() <= joy.radius) {
        joy.id = id;
        joy.stick = {x, y};
        return;
    }
    
    Vec2 aDist = { x - atkBtn.pos.x, y - atkBtn.pos.y };
    if (aDist.length() <= atkBtn.radius) {
        atkBtn.id = id;
        atkBtn.hold = true;
    }
}

void Controls::touchMoved(int id, float x, float y) {
    if (joy.id == id) {
        Vec2 dist = { x - joy.center.x, y - joy.center.y };
        if (dist.length() > joy.radius) {
            dist = dist.normalize();
            dist.x *= joy.radius;
            dist.y *= joy.radius;
        }
        joy.stick = { joy.center.x + dist.x, joy.center.y + dist.y };
    }
}

void Controls::touchReleased(int id) {
    if (joy.id == id) {
        joy.id = -1;
        joy.stick = joy.center;
    }
    if (atkBtn.id == id) {
        atkBtn.id = -1;
        atkBtn.hold = false;
    }
}

void Controls::keyDown(int key) {
    if (key == 51) keys.w = true; // AKEYCODE_A
    if (key == 29) keys.a = true; // AKEYCODE_A - исправь коды под себя или используй SDL
    if (key == 47) keys.s = true;
    if (key == 32) keys.d = true;
    if (key == 62) { keys.space = true; spaceJustPressed = true; }
}

void Controls::keyUp(int key) {
    if (key == 51) keys.w = false;
    if (key == 29) keys.a = false;
    if (key == 47) keys.s = false;
    if (key == 32) keys.d = false;
    if (key == 62) keys.space = false;
}
