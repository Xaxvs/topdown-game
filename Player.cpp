#include "Player.h"

#include <raymath.h>

#include <cmath>

#include "units.h"

Player::Player(b2WorldId world, int width, int height)
  : states_(this)
{
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

  sword_texture_ = Rectangle {
    kGridSize * 2,    kGridSize * 9,
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

b2BodyId Player::GetPlayerBody() const {
  return player_body_;
}

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
        // enemies.AttackEnemy(enemy.value(), GetWeapon().damage_, x, y);
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
  SetPosition(Vec(b2Body_GetPosition(player_body_)));
  states_.Update();
}

void Player::Draw(Texture2D tilemap) {  
  states_.Draw(position_, Vector2 { 32, 32 }, { 64, 64 }, current_angle_ + PI / 2);
  switch (states_.GetCurrentState()) {
    case PlayerStates::IState::kIAttackState:
      DrawText("Attack 1", 0, 0, 24, BLACK);
      break;
    case PlayerStates::IState::kIStateIdle:
      DrawText("Idle", 0, 0, 24, BLACK);
      break;
    case PlayerStates::IState::kIAttack2State:
      DrawText("Attack 2", 0, 0, 24, BLACK);
      break;
    case PlayerStates::IState::kIAttack3State:
      DrawText("Attack 3", 0, 0, 24, BLACK);
      break;
  }
}

float Player::GetHeading(void) const {
  return current_angle_;
}

void Player::SetHeading(float heading) {
  current_angle_ = heading;
}

Vector2 Player::GetPosition() const {
  return position_;
}
