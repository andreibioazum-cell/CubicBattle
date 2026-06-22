#pragma once
#include "math_utils.h"

struct Joystick {
    int id;
    Vec2 center;
    Vec2 stick;
    float radius;
    float stickRadius;
};

struct TouchButton {
    int id;
    Vec2 pos;
    float radius;
    bool hold;
    float pressAnim; // 0 to 1
};

struct KeyState {
    bool w, a, s, d, space;
};

class Controls {
public:
    Joystick joy;
    TouchButton atkBtn;
    TouchButton backBtn; // Используем radius для определения площади
    KeyState keys;
    Vec2 aimDir;
    bool spaceJustPressed;
    bool isMobile;

    Controls();
    void place(int w, int h);
    void update(float dt);
    Vec2 getMove();
    bool getShot(Vec2& outAim);
    
    // Ввод
    void touchPressed(int id, float x, float y);
    void touchMoved(int id, float x, float y);
    void touchReleased(int id);
    void keyDown(int key);
    void keyUp(int key);
};
