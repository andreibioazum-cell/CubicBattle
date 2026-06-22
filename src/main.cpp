#include "engine.h"
#include "game.h"
#include <android/log.h>
#include <android/keycodes.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "CubicBattle", __VA_ARGS__)

static Engine engine;
static Game game;

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int action = AMotionEvent_getAction(event);
        int flags = action & AMOTION_EVENT_ACTION_MASK;
        int id = AMotionEvent_getPointerId(event, 0);
        float x = AMotionEvent_getX(event, 0);
        float y = AMotionEvent_getY(event, 0);

        if (flags == AMOTION_EVENT_ACTION_DOWN) {
            game.controls.touchPressed(id, x, y);
        } else if (flags == AMOTION_EVENT_ACTION_MOVE) {
            game.controls.touchMoved(id, x, y);
        } else if (flags == AMOTION_EVENT_ACTION_UP) {
            game.controls.touchReleased(id);
        }
        return 1;
    }
    
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
        int action = AKeyEvent_getAction(event);
        int code = AKeyEvent_getKeyCode(event);
        if (action == AKEY_EVENT_ACTION_DOWN) game.controls.keyDown(code);
        if (action == AKEY_EVENT_ACTION_UP) game.controls.keyUp(code);
        return 1;
    }
    return 0;
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            engine.app = app;
            if (engine.app->window != NULL) {
                engine.initDisplay();
                game.init(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            engine.termDisplay();
            break;
    }
}

void android_main(struct android_app* state) {
    engine.app = state;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;

    int events;
    struct android_poll_source* source;
    
    while (1) {
        while (ALooper_pollAll(0, NULL, &events, (void**)&source) >= 0) {
            if (source != NULL) source->process(state, source);
            if (state->destroyRequested != 0) return;
        }
        
        if (engine.display != EGL_NO_DISPLAY) {
            game.update(1.0f / 60.0f); // Упрощенный dt
            game.draw(engine);
        }
    }
}
