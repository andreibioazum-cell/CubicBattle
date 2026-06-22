#pragma once
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

struct Engine {
    struct android_app* app;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int width;
    int height;
    
    Engine() : app(nullptr), display(EGL_NO_DISPLAY), surface(EGL_NO_SURFACE), context(EGL_NO_CONTEXT), width(0), height(0) {}
    
    int initDisplay();
    void termDisplay();
    void drawFrame();
};
