//
// Created by adria on 24/02/2019.
//

#ifndef SPACEINVADERS_KEYCODEMAP_HPP
#define SPACEINVADERS_KEYCODEMAP_HPP

#include <SDL2/SDL_keycode.h>

namespace GameEngine {
    namespace Internal {

        //TODO: Map of the GameEngine keys and SDL keycodes
// https://github.com/ocornut/imgui/blob/master/examples/imgui_impl_sdl.cpp
        class KeyMap {
            std::map<SDL_Keycode, KeyCode> map;
        public:
            KeyMap();
            KeyCode operator[](SDL_Keycode code){
                return map[code];
            };
        };

        KeyMap::KeyMap(){
            map[SDLK_UNKNOWN] = KeyCode::KEY_UNKNOWN;
            map[SDLK_RETURN] = KeyCode::KEY_RETURN;
            map[SDLK_ESCAPE] = KeyCode::KEY_ESCAPE;
            map[SDLK_BACKSPACE] = KeyCode::KEY_BACKSPACE;
            map[SDLK_TAB] = KeyCode::KEY_TAB;
            map[SDLK_SPACE] = KeyCode::KEY_SPACE;
            map[SDLK_EXCLAIM] = KeyCode::KEY_EXCLAIM;
            map[SDLK_QUOTEDBL] = KeyCode::KEY_QUOTEDBL;
            map[SDLK_HASH] = KeyCode::KEY_HASH;
            map[SDLK_PERCENT] = KeyCode::KEY_PERCENT;
            map[SDLK_DOLLAR] = KeyCode::KEY_DOLLAR;
            map[SDLK_AMPERSAND] = KeyCode::KEY_AMPERSAND;
            map[SDLK_QUOTE] = KeyCode::KEY_QUOTE;
            map[SDLK_LEFTPAREN] = KeyCode::KEY_LEFTPAREN;
            map[SDLK_RIGHTPAREN] = KeyCode::KEY_RIGHTPAREN;
            map[SDLK_ASTERISK] = KeyCode::KEY_ASTERISK;
            map[SDLK_PLUS] = KeyCode::KEY_PLUS;
            map[SDLK_COMMA] = KeyCode::KEY_COMMA;
            map[SDLK_MINUS] = KeyCode::KEY_MINUS;
            map[SDLK_PERIOD] = KeyCode::KEY_PERIOD;
            map[SDLK_SLASH] = KeyCode::KEY_SLASH;
            map[SDLK_0] = KeyCode::KEY_0;
            map[SDLK_1] = KeyCode::KEY_1;
            map[SDLK_2] = KeyCode::KEY_2;
            map[SDLK_3] = KeyCode::KEY_3;
            map[SDLK_4] = KeyCode::KEY_4;
            map[SDLK_5] = KeyCode::KEY_5;
            map[SDLK_6] = KeyCode::KEY_6;
            map[SDLK_7] = KeyCode::KEY_7;
            map[SDLK_8] = KeyCode::KEY_8;
            map[SDLK_9] = KeyCode::KEY_9;
            map[SDLK_COLON] = KeyCode::KEY_COLON;
            map[SDLK_SEMICOLON] = KeyCode::KEY_SEMICOLON;
            map[SDLK_LESS] = KeyCode::KEY_LESS;
            map[SDLK_EQUALS] = KeyCode::KEY_EQUALS;
            map[SDLK_GREATER] = KeyCode::KEY_GREATER;
            map[SDLK_QUESTION] = KeyCode::KEY_QUESTION;
            map[SDLK_AT] = KeyCode::KEY_AT;
            map[SDLK_LEFTBRACKET] = KeyCode::KEY_LEFTBRACKET;
            map[SDLK_BACKSLASH ] = KeyCode::KEY_BACKSLASH ;
            map[SDLK_RIGHTBRACKET] = KeyCode::KEY_RIGHTBRACKET;
            map[SDLK_CARET] = KeyCode::KEY_CARET;
            map[SDLK_UNDERSCORE] = KeyCode::KEY_UNDERSCORE;
            map[SDLK_BACKQUOTE] = KeyCode::KEY_BACKQUOTE;
            map[SDLK_a] = KeyCode::KEY_a;
            map[SDLK_b] = KeyCode::KEY_b;
            map[SDLK_c] = KeyCode::KEY_c;
            map[SDLK_d] = KeyCode::KEY_d;
            map[SDLK_e] = KeyCode::KEY_e;
            map[SDLK_f] = KeyCode::KEY_f;
            map[SDLK_g] = KeyCode::KEY_g;
            map[SDLK_h] = KeyCode::KEY_h;
            map[SDLK_i] = KeyCode::KEY_i;
            map[SDLK_j] = KeyCode::KEY_j;
            map[SDLK_k] = KeyCode::KEY_k;
            map[SDLK_l] = KeyCode::KEY_l;
            map[SDLK_m] = KeyCode::KEY_m;
            map[SDLK_n] = KeyCode::KEY_n;
            map[SDLK_o] = KeyCode::KEY_o;
            map[SDLK_p] = KeyCode::KEY_p;
            map[SDLK_q] = KeyCode::KEY_q;
            map[SDLK_r] = KeyCode::KEY_r;
            map[SDLK_s] = KeyCode::KEY_s;
            map[SDLK_t] = KeyCode::KEY_t;
            map[SDLK_u] = KeyCode::KEY_u;
            map[SDLK_v] = KeyCode::KEY_v;
            map[SDLK_w] = KeyCode::KEY_w;
            map[SDLK_x] = KeyCode::KEY_x;
            map[SDLK_y] = KeyCode::KEY_y;
            map[SDLK_z] = KeyCode::KEY_z;
            map[SDLK_F1] = KeyCode::KEY_F1;
            map[SDLK_F2] = KeyCode::KEY_F2;
            map[SDLK_F3] = KeyCode::KEY_F3;
            map[SDLK_F4] = KeyCode::KEY_F4;
            map[SDLK_F5] = KeyCode::KEY_F5;
            map[SDLK_F6] = KeyCode::KEY_F6;
            map[SDLK_F7] = KeyCode::KEY_F7;
            map[SDLK_F8] = KeyCode::KEY_F8;
            map[SDLK_F9] = KeyCode::KEY_F9;
            map[SDLK_F10] = KeyCode::KEY_F10;
            map[SDLK_F11] = KeyCode::KEY_F11;
            map[SDLK_F12] = KeyCode::KEY_F12;
            map[SDLK_PRINTSCREEN] = KeyCode::KEY_PRINTSCREEN;
            map[SDLK_SCROLLLOCK] = KeyCode::KEY_SCROLLLOCK;
            map[SDLK_PAUSE] = KeyCode::KEY_PAUSE;
            map[SDLK_INSERT] = KeyCode::KEY_INSERT;
            map[SDLK_HOME] = KeyCode::KEY_HOME;
            map[SDLK_PAGEUP] = KeyCode::KEY_PAGEUP;
            map[SDLK_DELETE] = KeyCode::KEY_DELETE;
            map[SDLK_END] = KeyCode::KEY_END;
            map[SDLK_PAGEDOWN] = KeyCode::KEY_PAGEDOWN;
            map[SDLK_RIGHT] = KeyCode::KEY_RIGHT;
            map[SDLK_LEFT] = KeyCode::KEY_LEFT;
            map[SDLK_DOWN] = KeyCode::KEY_DOWN;
            map[SDLK_UP] = KeyCode::KEY_UP;
            map[SDLK_LCTRL] = KeyCode::KEY_LCTRL;
            map[SDLK_LSHIFT] = KeyCode::KEY_LSHIFT;
            map[SDLK_LALT] = KeyCode::KEY_LALT;
            map[SDLK_RCTRL] = KeyCode::KEY_RCTRL;
            map[SDLK_RSHIFT] = KeyCode::KEY_RSHIFT;
            map[SDLK_RALT] = KeyCode::KEY_RALT;
            map[SDLK_NUMLOCKCLEAR] = KeyCode::KEY_NUMLOCKCLEAR;
            map[SDLK_KP_DIVIDE] = KeyCode::NUMPAD_DIVIDE,
            map[SDLK_KP_MULTIPLY] = KeyCode::NUMPAD_MULTIPLY;
            map[SDLK_KP_MINUS] = KeyCode::NUMPAD_MINUS;
            map[SDLK_KP_PLUS] = KeyCode::NUMPAD_PLUS;
            map[SDLK_KP_ENTER] = KeyCode::NUMPAD_ENTER;
            map[SDLK_KP_1] = KeyCode::NUMPAD_1;
            map[SDLK_KP_2] = KeyCode::NUMPAD_2;
            map[SDLK_KP_3] = KeyCode::NUMPAD_3;
            map[SDLK_KP_4] = KeyCode::NUMPAD_4;
            map[SDLK_KP_5] = KeyCode::NUMPAD_5;
            map[SDLK_KP_6] = KeyCode::NUMPAD_6;
            map[SDLK_KP_7] = KeyCode::NUMPAD_7;
            map[SDLK_KP_8] = KeyCode::NUMPAD_8;
            map[SDLK_KP_9] = KeyCode::NUMPAD_9;
            map[SDLK_KP_0] = KeyCode::NUMPAD_0;
            map[SDLK_KP_PERIOD] = KeyCode::NUMPAD_PERIOD;
        };

    }
}
#endif //SPACEINVADERS_KEYCODEMAP_HPP
