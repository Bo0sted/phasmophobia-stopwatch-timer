#include "uioeventloop.h"
#include <QDebug>
#include <thread>

static UioEventLoop* g_instance = nullptr;

UioEventLoop::UioEventLoop(QObject *parent)
    : QObject(parent) {
    g_instance = this;

    hook_set_dispatch_proc(UioEventLoop::dispatch_proc);

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
    if (event->type == EVENT_KEY_PRESSED) {
        emit keyPressed(event->data.keyboard.keycode);
    } else if (event->type == EVENT_KEY_RELEASED) {
        emit keyReleased(event->data.keyboard.keycode);
    }
}
