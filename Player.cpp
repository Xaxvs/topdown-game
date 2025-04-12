#include "Player.h"

#include "Pause.h"

#include <cmath>
#include <iostream>

const std::string &Player::GetDisplayName() const { return display_name_; }

Item &Player::GetShield() { return shield_; }

Item &Player::GetWeapon() { return weapon_; }

void Player::SetShield(const Item &shield) { shield_ = shield; }

void Player::SetWeapon(const Item &weapon) { weapon_ = weapon; }

int Player::GetX() const { return x_; }

int Player::GetY() const { return y_; }

int Player::SetX(const int value) { return x_ = value; }

int Player::SetY(const int value) { return y_ = value; }

int &Player::GetMoney() {
  return money_;
}


void Player::Attack(Enemies& enemies) {

  Vector2 char_pos = {float(x_ + width_ / 2), float(y_ - height_ / 2)};
  Vector2 mouse_pos = GetMousePosition();
  float target_angle = std::atan2(char_pos.y - mouse_pos.y , char_pos.x - mouse_pos.x);

  if (attack_cooldown_ <= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

    if (ray_angle_ > target_angle + PI / 4) {
      attack_cooldown_ = kDefaultAttackCooldown;
      ray_angle_ = target_angle - PI / 4;
      return;
    }

    constexpr float maxDistance = 100.0f;
    float x = char_pos.x, y = char_pos.y;
    float traveled = 0.0f;
    while (traveled < maxDistance) {
      x -= std::cos(ray_angle_);
      y -= std::sin(ray_angle_);
      DrawRectangle(x, y, 2, 2, BLACK);

      if (auto enemy = enemies.GetEnemyAt(x, y); enemy.has_value()) {
        enemies.AttackEnemy(enemy.value(), GetWeapon().damage_, x_, y_);
        attack_cooldown_ = kDefaultAttackCooldown;
      }
      traveled++;
    }
    ray_angle_ += 0.009;
  } else if(IsMouseButtonUp(MOUSE_BUTTON_LEFT) && ray_angle_ > 0.4) {
    attack_cooldown_ = kDefaultAttackCooldown;
    ray_angle_ = 0;
  }
  attack_cooldown_ -= GetFrameTime();
}

/*
 * If we really want to we can add
 * velocity and what not, but since we
 * are on a tight schedule I'll just leave
 * it like this.
 */
void Player::Update() {
  Vector2 mouse_pos =  GetMousePosition();
  float target_angle = atan2(mouse_pos.y - y_ + height_ / 2 , mouse_pos.x - x_ + width_ / 2);

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    x_ += std::cos(target_angle) * 0.2f;
    y_ += std::sin(target_angle) * 0.2f;
  }

  DrawRectanglePro({x_ + width_ / 2, y_ - height_ / 2, float(width_), float(height_)},
                   {float(width_) / 2, float(height_) / 2},
                   target_angle * (180 / PI),
                   BLACK);
}
