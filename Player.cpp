#include "player.h"

#include <cmath>

const std::string &Player::GetDisplayName() const { return m_display_name; }

Item &Player::GetShield() { return m_shield; }

Item &Player::GetWeapon() { return m_weapon; }

void Player::SetShield(const Item &shield) { m_shield = shield; }

void Player::SetWeapon(const Item &weapon) { m_weapon = weapon; }

int Player::GetX() const { return m_x; }

int Player::GetY() const { return m_y; }

int Player::SetX(const int value) { return m_x = value; }

int Player::SetY(const int value) { return m_y = value; }

void Player::attack(Enemies& enemies) {
  if(attackCooldown <= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (m_rayAngle > 2 * PI) {
      attackCooldown = DEFAULT_ATTACK_COOLDOWN;
      m_rayAngle = 0;
      return;
    }
    constexpr float maxDistance = 100.0f;

    Vector2 pos = GetMousePosition();
    Vector2 char_pos = {float(m_x + m_width / 2), float(m_y + m_height / 2)};

    //double dx = pos.x - char_pos.x;
    //double dy = pos.y - char_pos.y;

    //double angle = atan2(dy, dx);

    double x = char_pos.x, y = char_pos.y;
    float traveled = 0.0f;
    while(traveled < maxDistance) {
      x += cos(m_rayAngle); // cos(angle);
      y += sin(m_rayAngle); // sin(angle);
      DrawRectangle(x, y, 1 , 1, BLACK);


      if(auto enemy = enemies.getEnemyAt(x,y); enemy.has_value()){
        enemies.kill(enemy.value());
      }
      // something like if(EnemyExists(x,y)) GetEnemyAtPoint(x,y).Kill();
      traveled++;
    }

    m_rayAngle += 0.01f;  // Adjust rotation speed
    //signifies a full rotation, maybe just stop the animation or something.
    //m_rayAngle -= 2 * PI;
  }else if(IsMouseButtonUp(MOUSE_BUTTON_LEFT) && m_rayAngle > 1 && m_rayAngle < 2* PI) {
    attackCooldown = DEFAULT_ATTACK_COOLDOWN;
    m_rayAngle = 0;
  }
  attackCooldown--;
}

/*
 * If we really want to we can add
 * velocity and what not, but since we
 * are on a tight schedule I'll just leave
 * it like this.
 */
void Player::Update() {

  DrawRectangle(m_x, m_y, m_width, m_height, BLACK);

  m_velX = 0;
  m_velY = 0;

  if (IsKeyDown(KEY_W)) m_velY = -0.25;
  if (IsKeyDown(KEY_S)) m_velY = 0.25;
  if (IsKeyDown(KEY_D)) m_velX = 0.25;
  if (IsKeyDown(KEY_A)) m_velX = -0.25;

  m_x += m_velX;
  m_y += m_velY;
}