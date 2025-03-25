#include "Player.h"

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

void Player::Attack(Enemies& enemies) {
  if(attack_cooldown_ <= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (ray_angle_ > 2 * PI) {
      attack_cooldown_ = kDefaultAttackCooldown;
      ray_angle_ = 0;
      return;
    }

    constexpr float maxDistance = 100.0f;
    Vector2 char_pos = {float(x_ + width_ / 2), float(y_ + height_ / 2)};

    double x = char_pos.x, y = char_pos.y;
    float traveled = 0.0f;
    while(traveled < maxDistance) {
      x += cos(ray_angle_);
      y += sin(ray_angle_);
      DrawRectangle(x, y, 2 , 2, BLACK);

      if(auto enemy = enemies.GetEnemyAt(x,y); enemy.has_value()){
        enemies.AttackEnemy(enemy.value(), GetWeapon().damage_);
        attack_cooldown_ = kDefaultAttackCooldown;
        ray_angle_ = 0;
        return;
      }
      traveled++;
    }

    ray_angle_ += 0.01f;
  }else if(IsMouseButtonUp(MOUSE_BUTTON_LEFT) && ray_angle_ > 1 && ray_angle_ < 2* PI) {
    attack_cooldown_ = kDefaultAttackCooldown;
    ray_angle_ = 0;
  }

  attack_cooldown_-= GetFrameTime();
}

/*
 * If we really want to we can add
 * velocity and what not, but since we
 * are on a tight schedule I'll just leave
 * it like this.
 */
void Player::Update() {

  DrawRectangle(x_, y_, width_, height_, BLACK);

  vel_x_ = 0;
  vel_y_ = 0;

  if (IsKeyDown(KEY_W)) vel_y_ = -0.25;
  if (IsKeyDown(KEY_S)) vel_y_ = 0.25;
  if (IsKeyDown(KEY_D)) vel_x_ = 0.25;
  if (IsKeyDown(KEY_A)) vel_x_ = -0.25;

  x_ += vel_x_;
  y_ += vel_y_;
}