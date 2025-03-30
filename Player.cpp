#include "Player.h"

#include <raymath.h>

#include <cmath>

#include "units.h"

Player::Player(b2WorldId world, const std::string& display_name, int width, int height) {
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

  b2BodyDef body_def = b2DefaultBodyDef();
  body_def.type = b2BodyType::b2_dynamicBody;
  body_def.fixedRotation = true;
  body_def.position = b2Vec(position_);
  player_body_ = b2CreateBody(world, &body_def);

  constexpr float kDefaultRadius = 25.0f;
  
  b2ShapeDef shape_def = b2DefaultShapeDef();
  shape_def.density = 0.5;
  shape_def.friction = 0.4;
  
  b2Circle circle = b2Circle { b2Vec2_zero, kDefaultRadius * kPixelToMeter };
  player_shape_ = b2CreateCircleShape(player_body_, &shape_def, &circle);

  current_angle_ = 0.0f;
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
  b2Vec2 input = b2Vec2_zero;
  if (IsKeyDown(KEY_W)) {
    input.y -= 1.0;
  }
  
  if (IsKeyDown(KEY_A)) {
    input.x -= 1.0;
  }
  
  if (IsKeyDown(KEY_S)) {
    input.y += 1.0;
  }
  
  if (IsKeyDown(KEY_D)) {
    input.x += 1.0;
  }

  input = b2Normalize(input);

  float input_length = b2Length(input);

  if (input_length > 0.f) {
    target_angle_ = atan2(input.y, input.x);
    target_angle_ = Wrap(target_angle_, -PI, PI);
  }
  
  float delta = target_angle_ - current_angle_;
  if (delta > PI) {
    current_angle_ += 2 * PI;
  } else if (delta < -PI) {
    current_angle_ -= 2 * PI;
  }

  current_angle_ = Lerp(current_angle_, target_angle_, 0.1);
  current_angle_ = Wrap(current_angle_, -PI, PI);
    
  b2Vec2 current_velocity = b2Body_GetLinearVelocity(player_body_);
  if (input_length > 0.f) {
    float speed = 4.0f;
    b2Vec2 new_velocity = b2Lerp(current_velocity, b2MulSV(speed, input), 0.1);
    b2Body_SetLinearVelocity(player_body_, new_velocity);
  } else {
    b2Vec2 new_velocity = b2Lerp(current_velocity, b2Vec2_zero, 0.6);
    b2Body_SetLinearVelocity(player_body_, new_velocity);
  }

  position_ = Vec(b2Body_GetPosition(player_body_));
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
    current_angle_ * RAD2DEG,
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

  r_hand_rect.x += cosf(current_angle_ + PI/2) * 30.0;
  r_hand_rect.y += sin(current_angle_ + PI/2) * 30.0;
  
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

  l_hand_rect.x -= cosf(current_angle_ + PI/2) * 30.0;
  l_hand_rect.y -= sin(current_angle_ + PI/2) * 30.0;
  
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
  return current_angle_;
}

Vector2 Player::GetPosition() const {
  return position_;
}
