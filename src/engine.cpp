#include "engine.h"
#include <android/log.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "CubicBattle", __VA_ARGS__)

int Engine::initDisplay() {
    const EGLint attribs[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE };
    EGLint w, h, format;
    EGLConfig config;
    EGLint numConfigs;
    
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, 0, 0);
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    
    if (numConfigs == 0) {
        LOGE("EGL: Конфигурации не найдены");
        return -1;
    }

    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);
    
    surface = eglCreateWindowSurface(display, config, app->window, NULL);
    if (surface == EGL_NO_SURFACE) {
        LOGE("EGL: Ошибка создания поверхности");
        return -1;
    }

    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    context = eglCreateContext(display, config, NULL, contextAttribs);
    if (context == EGL_NO_CONTEXT) {
        LOGE("EGL: Ошибка создания контекста");
        return -1;
    }
    
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGE("EGL: Ошибка eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
    
    width = w;
    height = h;
    return 0;
}

void Engine::termDisplay() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
        if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

void Engine::drawFrame() {
    if (display == EGL_NO_DISPLAY) return;
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(display, surface);
}
