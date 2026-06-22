#pragma once
#include <android/native_activity.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

struct Engine {
    struct android_app* app;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int width;
    int height;
    
    int initDisplay();
    void termDisplay();
    void drawFrame();
};
