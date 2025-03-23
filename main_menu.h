
#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include <raylib.h>

#include "button.h"

class main_menu {

    main_menu() :
    m_play_button("PLAY", BLACK, 10.0f, 10.0f, 10, 10, WHITE),
    m_settings_button("SETTINGS", BLACK, 10.0f, 10.0f, 10, 10, WHITE),
    m_credits_button("CREDITS", BLACK, 10.0f, 10.0f, 10, 10, WHITE)
    {

    }

public:
    void draw();

private:
    void on_play_click();
    void on_settings_click();
    void on_credits_click();

private:
    Vector2 m_mouse_pos = {0.0f, 0.0f};
    button m_play_button;
    button m_settings_button;
    button m_credits_button;
};



#endif //MAIN_MENU_H
