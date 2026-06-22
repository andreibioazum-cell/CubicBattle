#pragma once
#include <cmath>

struct Vec2 {
    float x, y;
    
    float length() const {
        return std::sqrt(x*x + y*y);
    }
    
    Vec2 normalize() const {
        float len = length();
        if (len > 0) return {x/len, y/len};
        return {0, 0};
    }
};
