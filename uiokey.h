#pragma once
#include <QString>
#include "uiohook.h"


inline bool IsKeycodeModifierKey(int keycode, int rawcode) {
#ifdef Q_OS_LINUX
    switch (rawcode) {
    case 65505:
    case 65507:
    case 65515:
    case 65513:
    case 65506:
    case 65508:
    case 65516:
    case 65514:
        return true;
    }
#endif
    switch (keycode) {
    case VC_SHIFT_L:
    case VC_SHIFT_R:
    case VC_CONTROL_L:
    case VC_CONTROL_R:
    case VC_ALT_L:
    case VC_ALT_R:
    case VC_META_L:
    case VC_META_R:
    case VC_CAPS_LOCK:
        return true;
    default:
        return false;
    }
}

enum class UioKey {
    NONE        = VC_UNDEFINED,
    ESCAPE      = VC_ESCAPE,
    ONE         = VC_1,
    TWO         = VC_2,
    THREE       = VC_3,
    FOUR        = VC_4,
    FIVE        = VC_5,
    SIX         = VC_6,
    SEVEN       = VC_7,
    EIGHT       = VC_8,
    NINE        = VC_9,
    ZERO        = VC_0,
    MINUS       = VC_MINUS,
    EQUALS      = VC_EQUALS,
    BACKSPACE   = VC_BACKSPACE,
    TAB         = VC_TAB,
    Q           = VC_Q,
    W           = VC_W,
    E           = VC_E,
    R           = VC_R,
    T           = VC_T,
    Y           = VC_Y,
    U           = VC_U,
    I           = VC_I,
    O           = VC_O,
    P           = VC_P,
    LBRACKET    = VC_OPEN_BRACKET,
    RBRACKET    = VC_CLOSE_BRACKET,
    ENTER       = VC_ENTER,
    LCTRL       = VC_CONTROL_L,
    A           = VC_A,
    S           = VC_S,
    D           = VC_D,
    F           = VC_F,
    G           = VC_G,
    H           = VC_H,
    J           = VC_J,
    K           = VC_K,
    L           = VC_L,
    SEMICOLON   = VC_SEMICOLON,
    APOSTROPHE  = VC_QUOTE,
    GRAVE       = VC_BACKQUOTE,
    LSHIFT      = VC_SHIFT_L,
    BACKSLASH   = VC_BACK_SLASH,
    Z           = VC_Z,
    X           = VC_X,
    C           = VC_C,
    V           = VC_V,
    B           = VC_B,
    N           = VC_N,
    M           = VC_M,
    COMMA       = VC_COMMA,
    PERIOD      = VC_PERIOD,
    SLASH       = VC_SLASH,
    RSHIFT      = VC_SHIFT_R,
    KP_MULTIPLY = VC_KP_MULTIPLY,
    LALT        = VC_ALT_L,
    SPACE       = VC_SPACE,
    CAPS_LOCK   = VC_CAPS_LOCK,
    F1          = VC_F1,
    F2          = VC_F2,
    F3          = VC_F3,
    F4          = VC_F4,
    F5          = VC_F5,
    F6          = VC_F6,
    F7          = VC_F7,
    F8          = VC_F8,
    F9          = VC_F9,
    F10         = VC_F10,
    NUM_LOCK    = VC_NUM_LOCK,
    SCROLL_LOCK = VC_SCROLL_LOCK,
    KP_7        = VC_KP_7,
    KP_8        = VC_KP_8,
    KP_9        = VC_KP_9,
    KP_MINUS    = VC_KP_SUBTRACT,
    KP_4        = VC_KP_4,
    KP_5        = VC_KP_5,
    KP_6        = VC_KP_6,
    KP_PLUS     = VC_KP_ADD,
    KP_1        = VC_KP_1,
    KP_2        = VC_KP_2,
    KP_3        = VC_KP_3,
    KP_0        = VC_KP_0,
    KP_PERIOD   = VC_KP_SEPARATOR,
    F11         = VC_F11,
    F12         = VC_F12,
    KP_DIVIDE   = VC_KP_DIVIDE,
    KP_ENTER    = VC_KP_ENTER,
    RCTRL       = VC_CONTROL_R,
    RALT        = VC_ALT_R,
    HOME        = VC_HOME,
    UP          = VC_UP,
    PAGE_UP     = VC_PAGE_UP,
    LEFT        = VC_LEFT,
    RIGHT       = VC_RIGHT,
    END         = VC_END,
    DOWN        = VC_DOWN,
    PAGE_DOWN   = VC_PAGE_DOWN,
    INSERT      = VC_INSERT,
    DELETE_KEY  = VC_DELETE,
    LMETA       = VC_META_L,
    RMETA       = VC_META_R
};

inline QString UioKeyToQString(UioKey key) {
    switch (key) {
    case UioKey::NONE: return "Unknown";
    case UioKey::ESCAPE: return "Escape";
    case UioKey::ONE: return "1";
    case UioKey::TWO: return "2";
    case UioKey::THREE: return "3";
    case UioKey::FOUR: return "4";
    case UioKey::FIVE: return "5";
    case UioKey::SIX: return "6";
    case UioKey::SEVEN: return "7";
    case UioKey::EIGHT: return "8";
    case UioKey::NINE: return "9";
    case UioKey::ZERO: return "0";
    case UioKey::MINUS: return "-";
    case UioKey::EQUALS: return "=";
    case UioKey::BACKSPACE: return "Backspace";
    case UioKey::TAB: return "Tab";
    case UioKey::Q: return "Q";
    case UioKey::W: return "W";
    case UioKey::E: return "E";
    case UioKey::R: return "R";
    case UioKey::T: return "T";
    case UioKey::Y: return "Y";
    case UioKey::U: return "U";
    case UioKey::I: return "I";
    case UioKey::O: return "O";
    case UioKey::P: return "P";
    case UioKey::LBRACKET: return "[";
    case UioKey::RBRACKET: return "]";
    case UioKey::ENTER: return "Enter";
    case UioKey::LCTRL: return "Left Ctrl";
    case UioKey::A: return "A";
    case UioKey::S: return "S";
    case UioKey::D: return "D";
    case UioKey::F: return "F";
    case UioKey::G: return "G";
    case UioKey::H: return "H";
    case UioKey::J: return "J";
    case UioKey::K: return "K";
    case UioKey::L: return "L";
    case UioKey::SEMICOLON: return ";";
    case UioKey::APOSTROPHE: return "'";
    case UioKey::GRAVE: return "`";
    case UioKey::LSHIFT: return "Left Shift";
    case UioKey::BACKSLASH: return "\\";
    case UioKey::Z: return "Z";
    case UioKey::X: return "X";
    case UioKey::C: return "C";
    case UioKey::V: return "V";
    case UioKey::B: return "B";
    case UioKey::N: return "N";
    case UioKey::M: return "M";
    case UioKey::COMMA: return ",";
    case UioKey::PERIOD: return ".";
    case UioKey::SLASH: return "/";
    case UioKey::RSHIFT: return "Right Shift";
    case UioKey::KP_MULTIPLY: return "Numpad *";
    case UioKey::LALT: return "Left Alt";
    case UioKey::SPACE: return "Space";
    case UioKey::CAPS_LOCK: return "Caps Lock";
    case UioKey::F1: return "F1";
    case UioKey::F2: return "F2";
    case UioKey::F3: return "F3";
    case UioKey::F4: return "F4";
    case UioKey::F5: return "F5";
    case UioKey::F6: return "F6";
    case UioKey::F7: return "F7";
    case UioKey::F8: return "F8";
    case UioKey::F9: return "F9";
    case UioKey::F10: return "F10";
    case UioKey::NUM_LOCK: return "Num Lock";
    case UioKey::SCROLL_LOCK: return "Scroll Lock";
    case UioKey::KP_7: return "Numpad 7";
    case UioKey::KP_8: return "Numpad 8";
    case UioKey::KP_9: return "Numpad 9";
    case UioKey::KP_MINUS: return "Numpad -";
    case UioKey::KP_4: return "Numpad 4";
    case UioKey::KP_5: return "Numpad 5";
    case UioKey::KP_6: return "Numpad 6";
    case UioKey::KP_PLUS: return "Numpad +";
    case UioKey::KP_1: return "Numpad 1";
    case UioKey::KP_2: return "Numpad 2";
    case UioKey::KP_3: return "Numpad 3";
    case UioKey::KP_0: return "Numpad 0";
    case UioKey::KP_PERIOD: return "Numpad .";
    case UioKey::F11: return "F11";
    case UioKey::F12: return "F12";
    case UioKey::KP_DIVIDE: return "Numpad /";
    case UioKey::KP_ENTER: return "Numpad Enter";
    case UioKey::RCTRL: return "Right Ctrl";
    case UioKey::RALT: return "Right Alt";
    case UioKey::HOME: return "Home";
    case UioKey::UP: return "Up";
    case UioKey::PAGE_UP: return "Page Up";
    case UioKey::LEFT: return "Left";
    case UioKey::RIGHT: return "Right";
    case UioKey::END: return "End";
    case UioKey::DOWN: return "Down";
    case UioKey::PAGE_DOWN: return "Page Down";
    case UioKey::INSERT: return "Insert";
    case UioKey::DELETE_KEY: return "Delete";
    case UioKey::LMETA: return "Left Meta";
    case UioKey::RMETA: return "Right Meta";
    }
    return "Unknown";
}

inline int translateLinuxRawcodeToKeycode(int rawcode) {
    switch (rawcode) {
    case 65505: return VC_SHIFT_L;
    case 65507: return VC_CONTROL_L;
    case 65515: return VC_META_L;
    case 65513: return VC_ALT_L;
    case 65506: return VC_SHIFT_R;
    case 65508: return VC_CONTROL_R;
    case 65516: return VC_META_R;
    case 65514: return VC_ALT_R;
    default: return VC_UNDEFINED;
    }
}
inline UioKey IntToUioKey(int code) {
    switch (code) {
    case VC_UNDEFINED: return UioKey::NONE;
    case VC_ESCAPE: return UioKey::ESCAPE;
    case VC_1: return UioKey::ONE;
    case VC_2: return UioKey::TWO;
    case VC_3: return UioKey::THREE;
    case VC_4: return UioKey::FOUR;
    case VC_5: return UioKey::FIVE;
    case VC_6: return UioKey::SIX;
    case VC_7: return UioKey::SEVEN;
    case VC_8: return UioKey::EIGHT;
    case VC_9: return UioKey::NINE;
    case VC_0: return UioKey::ZERO;
    case VC_MINUS: return UioKey::MINUS;
    case VC_EQUALS: return UioKey::EQUALS;
    case VC_BACKSPACE: return UioKey::BACKSPACE;
    case VC_TAB: return UioKey::TAB;
    case VC_Q: return UioKey::Q;
    case VC_W: return UioKey::W;
    case VC_E: return UioKey::E;
    case VC_R: return UioKey::R;
    case VC_T: return UioKey::T;
    case VC_Y: return UioKey::Y;
    case VC_U: return UioKey::U;
    case VC_I: return UioKey::I;
    case VC_O: return UioKey::O;
    case VC_P: return UioKey::P;
    case VC_OPEN_BRACKET: return UioKey::LBRACKET;
    case VC_CLOSE_BRACKET: return UioKey::RBRACKET;
    case VC_ENTER: return UioKey::ENTER;
    case VC_CONTROL_L: return UioKey::LCTRL;
    case VC_A: return UioKey::A;
    case VC_S: return UioKey::S;
    case VC_D: return UioKey::D;
    case VC_F: return UioKey::F;
    case VC_G: return UioKey::G;
    case VC_H: return UioKey::H;
    case VC_J: return UioKey::J;
    case VC_K: return UioKey::K;
    case VC_L: return UioKey::L;
    case VC_SEMICOLON: return UioKey::SEMICOLON;
    case VC_QUOTE: return UioKey::APOSTROPHE;
    case VC_BACKQUOTE: return UioKey::GRAVE;
    case VC_SHIFT_L: return UioKey::LSHIFT;
    case VC_BACK_SLASH: return UioKey::BACKSLASH;
    case VC_Z: return UioKey::Z;
    case VC_X: return UioKey::X;
    case VC_C: return UioKey::C;
    case VC_V: return UioKey::V;
    case VC_B: return UioKey::B;
    case VC_N: return UioKey::N;
    case VC_M: return UioKey::M;
    case VC_COMMA: return UioKey::COMMA;
    case VC_PERIOD: return UioKey::PERIOD;
    case VC_SLASH: return UioKey::SLASH;
    case VC_SHIFT_R: return UioKey::RSHIFT;
    case VC_KP_MULTIPLY: return UioKey::KP_MULTIPLY;
    case VC_ALT_L: return UioKey::LALT;
    case VC_SPACE: return UioKey::SPACE;
    case VC_CAPS_LOCK: return UioKey::CAPS_LOCK;
    case VC_F1: return UioKey::F1;
    case VC_F2: return UioKey::F2;
    case VC_F3: return UioKey::F3;
    case VC_F4: return UioKey::F4;
    case VC_F5: return UioKey::F5;
    case VC_F6: return UioKey::F6;
    case VC_F7: return UioKey::F7;
    case VC_F8: return UioKey::F8;
    case VC_F9: return UioKey::F9;
    case VC_F10: return UioKey::F10;
    case VC_NUM_LOCK: return UioKey::NUM_LOCK;
    case VC_SCROLL_LOCK: return UioKey::SCROLL_LOCK;
    case VC_KP_7: return UioKey::KP_7;
    case VC_KP_8: return UioKey::KP_8;
    case VC_KP_9: return UioKey::KP_9;
    case VC_KP_SUBTRACT: return UioKey::KP_MINUS;
    case VC_KP_4: return UioKey::KP_4;
    case VC_KP_5: return UioKey::KP_5;
    case VC_KP_6: return UioKey::KP_6;
    case VC_KP_ADD: return UioKey::KP_PLUS;
    case VC_KP_1: return UioKey::KP_1;
    case VC_KP_2: return UioKey::KP_2;
    case VC_KP_3: return UioKey::KP_3;
    case VC_KP_0: return UioKey::KP_0;
    case VC_KP_SEPARATOR: return UioKey::KP_PERIOD;
    case VC_F11: return UioKey::F11;
    case VC_F12: return UioKey::F12;
    case VC_KP_DIVIDE: return UioKey::KP_DIVIDE;
    case VC_KP_ENTER: return UioKey::KP_ENTER;
    case VC_CONTROL_R: return UioKey::RCTRL;
    case VC_ALT_R: return UioKey::RALT;
    case VC_HOME: return UioKey::HOME;
    case VC_UP: return UioKey::UP;
    case VC_PAGE_UP: return UioKey::PAGE_UP;
    case VC_LEFT: return UioKey::LEFT;
    case VC_RIGHT: return UioKey::RIGHT;
    case VC_END: return UioKey::END;
    case VC_DOWN: return UioKey::DOWN;
    case VC_PAGE_DOWN: return UioKey::PAGE_DOWN;
    case VC_INSERT: return UioKey::INSERT;
    case VC_DELETE: return UioKey::DELETE_KEY;
    case VC_META_L: return UioKey::LMETA;
    case VC_META_R: return UioKey::RMETA;
    default: return UioKey::NONE;
    }
}

inline QString KeycodeToQString(int code) {
    return UioKeyToQString(IntToUioKey(code));
}
