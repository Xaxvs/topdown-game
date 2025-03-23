#include "player.h"

const std::string &Player::GetDisplayName() const { return m_display_name; }

Item &Player::GetShield() { return m_shield; }

Item &Player::GetWeapon() { return m_weapon; }

void Player::SetShield(const Item &shield) { m_shield = shield; }

void Player::SetWeapon(const Item &weapon) { m_weapon = weapon; }

int Player::GetX() const { return m_x; }

int Player::GetY() const { return m_y; }

int Player::SetX(const int value) { return m_x = value; }

int Player::SetY(const int value) { return m_y = value; }

/*
 * If we really want to we can add
 * velocity and what not, but since we
 * are on a tight schedule I'll just leave
 * it like this.
 */
void Player::Update(const KeyboardKey key_pressed) {
  if (key_pressed == KEY_W)
    m_y += 10;
  if (key_pressed == KEY_S)
    m_y -= 10;
  if (key_pressed == KEY_D)
    m_x += 10;
  if (key_pressed == KEY_A)
    m_x -= 10;
}