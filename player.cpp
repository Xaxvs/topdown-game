#include "player.h"

const char *player::get_display_name() const {
    return m_display_name;
}

item& player::get_shield() const {
    return m_shield;
}

item& player::get_weapon() const {
    return m_weapon;
}

void player::set_shield(const item& shield) const {
    m_shield = shield;
}

void player::set_weapon(const item& weapon) const {
    m_weapon = weapon;
}

int player::get_x() const {
    return m_x;
}

int player::get_y() const {
    return m_y;
}

int player::set_x(const int value) {
    return m_x = value;
}

int player::set_y(const int value) {
    return m_y = value;
}

/*
 * If we really want to we can add
 * velocity and what not, but since we
 * are on a tight schedule I'll just leave
 * it like this.
 */
void player::update(const KeyboardKey key_pressed) {
    if(key_pressed == KEY_W) m_y += 10;
    if(key_pressed == KEY_S) m_y -= 10;
    if(key_pressed == KEY_D) m_x += 10;
    if(key_pressed == KEY_A) m_x -= 10;
}