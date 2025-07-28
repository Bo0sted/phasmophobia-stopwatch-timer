#ifndef UIOEVENTLOOP_H
#define UIOEVENTLOOP_H

#include <QObject>
#include <uiohook.h>

class UioEventLoop : public QObject {
    Q_OBJECT
public:
    explicit UioEventLoop(QObject *parent = nullptr);
    ~UioEventLoop();

signals:
    void keyPressed(int keycode);
    void keyReleased(int keycode);
    void modifierPressed(int keycode);
    void modifierReleased(int keycode);
private:
    static void dispatch_proc(uiohook_event * const event);
    void handleEvent(uiohook_event * const event);

};

#endif // UIOEVENTLOOP_H
