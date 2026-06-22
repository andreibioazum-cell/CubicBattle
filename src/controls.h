#pragma once
namespace controls {
    void update(float dt);
    void getMove(float& dx, float& dy);
    bool getShot(float& aimDx, float& aimDy);
    void draw();
}
