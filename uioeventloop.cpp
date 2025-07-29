#include "uioeventloop.h"
#include "uiokey.h"

#include <QDebug>
#include <thread>

static UioEventLoop* g_instance = nullptr;

UioEventLoop::UioEventLoop(QObject *parent)
    : QObject(parent) {
    g_instance = this;

    hook_set_dispatch_proc(&UioEventLoop::dispatch_proc);

    // Start uiohook in a separate thread
    std::thread([]() {
        if (hook_run() != UIOHOOK_SUCCESS) {
            qWarning() << "Failed to start uiohook";
        }
    }).detach();
}

UioEventLoop::~UioEventLoop() {
    hook_stop(); // stop hook on destruction
    g_instance = nullptr;
}

void UioEventLoop::dispatch_proc(uiohook_event * const event) {
    if (g_instance) {
        g_instance->handleEvent(event);
    }
}

void UioEventLoop::handleEvent(uiohook_event * const event) {
    auto keycode = event->data.keyboard.keycode;
    auto rawcode = event->data.keyboard.rawcode;
    if (event->type == EVENT_KEY_PRESSED) {
        qDebug() << "rawcode:" << rawcode << " keycode:" << keycode;
        if (IsKeycodeModifierKey(event->data.keyboard.keycode, event->data.keyboard.rawcode)) {
            #ifdef Q_OS_LINUX
            keycode = translateLinuxRawcodeToKeycode(rawcode);
            #endif
            emit modifierPressed(keycode, rawcode);
            emit keyPressed(keycode, rawcode); // send as normal key too
        } else {
            emit keyPressed(keycode, rawcode);
        }
    }
    else if (event->type == EVENT_KEY_RELEASED) {
        if (IsKeycodeModifierKey(event->data.keyboard.keycode, event->data.keyboard.rawcode)) {
            #ifdef Q_OS_LINUX
            keycode = translateLinuxRawcodeToKeycode(rawcode);
            #endif
            emit modifierReleased(keycode, rawcode);
            emit keyReleased(keycode, rawcode);
        } else {
            emit keyReleased(keycode, rawcode);
        }
    }

}
