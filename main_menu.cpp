
#include "main_menu.h"

void main_menu::draw() {
    m_mouse_pos = GetMousePosition();
    DrawText("Topdown-Game", 200, 200, 50, BLACK);
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if(CheckCollisionPointRec(m_mouse_pos, m_play_button.get_rect()))
            on_play_click();
        if(CheckCollisionPointRec(m_mouse_pos, m_credits_button.get_rect()))
            on_credits_click();
        if(CheckCollisionPointRec(m_mouse_pos, m_settings_button.get_rect()))
            on_settings_click();
    }

    m_credits_button.update();
    m_play_button.update();
    m_settings_button.update();
}

void main_menu::on_credits_click() {
    //TODO
}

void main_menu::on_play_click() {
    //TODO
}

void main_menu::on_settings_click() {
    //TODO
}



