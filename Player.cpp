#include "Player.h"

#include <cmath>

Player::Player(const std::string& display_name, int width, int height) {
  display_name_ = display_name;
  weapon_ = Item { "Default Knife", 10, 5, ItemType::WEAPON };
  shield_ = Item { "Default Chestplate", 10, 5, ItemType::SHIELD };
  width_ = width;
  height_ = height;

  constexpr size_t kGridSize = 64;
  
  player_texture_ = Rectangle {
    0,
    kGridSize * 8,
    kGridSize,
    kGridSize  
  };

  hand_texture_ = Rectangle {
    kGridSize,
    kGridSize * 8,
    kGridSize,
    kGridSize  
  };

  target_angle_ = 0.0f;
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
  target_angle_ = atan2(
                         mouse_pos.y - (position_.y + (float)height_ / 2.0),
                         mouse_pos.x - (position_.x + (float)width_ / 2.0)
                       );

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    position_.x += std::cos(target_angle_) * 100.0f * GetFrameTime();
    position_.y += std::sin(target_angle_) * 100.0f * GetFrameTime();
  }  
}

void Player::Draw(Texture2D tilemap) {
  Rectangle player_rect = {
    position_.x,
    position_.y,
    (float)width_,
    (float)height_
  };
  
  DrawTexturePro(
    tilemap,
    player_texture_,
    player_rect,
    { (float)width_ / 2, (float)height_ / 2 },
    target_angle_ * RAD2DEG,
    WHITE
  );

  float half_width = (float)width_ / 2;
  float half_height= (float)width_ / 2;

  Rectangle r_hand_rect {
    position_.x - half_width,
    position_.y - half_height,
    (float)width_,
    (float)height_
  };

  r_hand_rect.x += cosf(target_angle_ + PI/2) * 30.0;
  r_hand_rect.y += sin(target_angle_ + PI/2) * 30.0;
  
  DrawTexturePro(
    tilemap,
    hand_texture_,
    r_hand_rect,
    {0.0, 0.0},
    0.0,
    WHITE
  );
  
  Rectangle l_hand_rect {
    position_.x - half_width,
    position_.y - half_height,
    (float)width_,
    (float)height_
  };

  l_hand_rect.x -= cosf(target_angle_ + PI/2) * 30.0;
  l_hand_rect.y -= sin(target_angle_ + PI/2) * 30.0;
  
  DrawTexturePro(
    tilemap,
    hand_texture_,
    l_hand_rect,
    {0.0, 0.0},
    0.0,
    WHITE
  );
}

float Player::GetHeading(void) const {
  return target_angle_;
}

Vector2 Player::GetPosition() const {
  return position_;
}
