#include "Player.h"

#include <cmath>

Player::Player(const std::string& display_name, int width, int height) {
  display_name_ = display_name;
  weapon_ = Item { "Default Knife", 10, 5, ItemType::WEAPON };
  shield_ = Item { "Default Chestplate", 10, 5, ItemType::SHIELD };
  width_ = width;
  height_ = height;
}

const std::string& Player::GetDisplayName() const { return display_name_; }

Item& Player::GetShield() { return shield_; }

Item& Player::GetWeapon() { return weapon_; }

void Player::SetShield(const Item& shield) { shield_ = shield; }

void Player::SetWeapon(const Item& weapon) { weapon_ = weapon; }

void Player::SetPosition(Vector2 position) {
  position_ = position;
}

void Player::Attack(Enemies& enemies) {
  Vector2 char_pos = { position_.x + (float)width_ / 2, position_.y - (float)height_ / 2};
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
        enemies.AttackEnemy(enemy.value(), GetWeapon().damage_, x, y);
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

void Player::Update() {
  Vector2 mouse_pos =  GetMousePosition();
  float target_angle = atan2(
                         mouse_pos.y - position_.y + (float)height_ / 2,
                         mouse_pos.x - position_.x + (float)width_ / 2
                       );

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    position_.x += std::cos(target_angle) * 0.2f;
    position_.y += std::sin(target_angle) * 0.2f;
  }

  Rectangle player_rect = {
    position_.x + (float)width_ / 2,
    position_.y - (float)height_ / 2,
    (float)width_,
    (float)height_
  };
  
  DrawRectanglePro(
    player_rect,
    { (float)width_ / 2, (float)height_ / 2 },
    target_angle * (180 / PI),
    BLACK
  );
}
