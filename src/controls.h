#pragma once
#include "math_utils.h"

struct Joystick {
    int id;
    Vec2 center;
    Vec2 stick;
    float radius;
    float stickRadius;
    
    Joystick() : id(-1), center({0,0}), stick({0,0}), radius(90.0f), stickRadius(36.0f) {}
};

struct TouchButton {
    int id;
    Vec2 pos;
    float radius;
    bool hold;
    float pressAnim;
    
    TouchButton() : id(-1), pos({0,0}), radius(104.0f), hold(false), pressAnim(0.0f) {}
};

struct KeyState {
    bool w, a, s, d, space;
    KeyState() : w(false), a(false), s(false), d(false), space(false) {}
};

class Controls {
public:
    Joystick joy;
    TouchButton atkBtn;
    KeyState keys;
    Vec2 aimDir;
    bool spaceJustPressed;

    Controls();
    void place(int w, int h);
    void update(float dt);
    Vec2 getMove();
    bool getShot(Vec2& outAim);
    
    void touchPressed(int id, float x, float y);
    void touchMoved(int id, float x, float y);
    void touchReleased(int id);
    void keyDown(int key);
    void keyUp(int key);
};
