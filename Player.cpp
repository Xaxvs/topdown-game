#include "Player.h"

#include <raymath.h>

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

  lock_on_target_ = b2_nullBodyId;
}

b2BodyId Player::GetPlayerBody() const {
  return player_body_;
}

void Player::SetPosition(Vector2 position) {
  position_ = position;
}

void Player::Update() {
  SetPosition(Vec(b2Body_GetPosition(player_body_)));
  states_.Update();

  if (b2Body_IsValid(lock_on_target_)) {    
    Vector2 locked_pos = Vec(b2Body_GetPosition(lock_on_target_));
    Vector2 dir = Vector2Subtract(position_, locked_pos);

    if (Vector2LengthSqr(dir) >= 525000.0f) {
      SetLockedOnBody(b2_nullBodyId);
      return;
    }
    
    dir = Vector2Normalize(dir);
    
    float intended = atan2(dir.y, dir.x) + PI;
    intended = Wrap(intended, -PI, PI);
    
    float delta = intended - current_angle_;
    if (delta < -PI) {
      current_angle_ -= 2 * PI;
    } else if (delta > PI) {
      current_angle_ += 2 * PI;
    }
    
    current_angle_ = Lerp(current_angle_, intended, 0.4);
  }

  CycleLockOn();
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
  
  // DrawCircle(
  //     position_.x,
  //     position_.y,
  //     256.0f,
  //     { 0, 0, 255, 100 }
  // );
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

bool IsOverlapping(b2ShapeId id, void* context) {        
    Player* player = (Player*)context;
    if (player == nullptr) return false;
    
    if (!b2Body_IsValid(player->GetLockedOnBody())) {
      player->SetLockedOnBody(b2Shape_GetBody(id));
      player->PushLockedOnTarget(b2Shape_GetBody(id));
    } else {
      b2Vec2 current_pos = b2Body_GetPosition(player->GetLockedOnBody());
      b2Vec2 next_pos = b2Body_GetPosition(b2Shape_GetBody(id));
      b2Vec2 player_pos = b2Vec(player->GetPosition());

      float current_dist = b2LengthSquared(b2Sub(player_pos, current_pos));
      float next_dist = b2LengthSquared(b2Sub(player_pos, next_pos));
      if (next_dist < current_dist) {
        player->SetLockedOnBody(b2Shape_GetBody(id));
      }
      player->PushLockedOnTarget(b2Shape_GetBody(id));
    }
    
    return true;
}

void Player::SetLockedOnBody(b2BodyId body) {
  if (!b2Body_IsValid(body)) {
    locked_on_targets_.clear();
  }
  lock_on_target_ = body;
}

b2BodyId Player::GetLockedOnBody() {
  return lock_on_target_;
}

void Player::LockOn(b2WorldId world) {
  b2Circle lock_detection = { b2Vec(position_), 256.0 * kPixelToMeter };

  b2QueryFilter filter = b2DefaultQueryFilter();
  filter.categoryBits = 0x002;
  filter.maskBits = 0x002;

  b2World_OverlapCircle(world, &lock_detection, b2Transform_identity, filter, IsOverlapping, this);
}

void Player::PushLockedOnTarget(b2BodyId target) {
  locked_on_targets_.push_back(target);
}

const std::vector<b2BodyId>& Player::GetTargetsInRange() const {
  return locked_on_targets_;
}

void Player::CycleLockOn() {
    if (IsKeyPressed(KEY_E) && b2Body_IsValid(GetLockedOnBody())) {
        b2BodyId current = GetLockedOnBody();
    
        float min_delta = std::numeric_limits<float>().max();
        b2BodyId min_angle_body = b2_nullBodyId;
    
        for (b2BodyId target : GetTargetsInRange()) {
            if (B2_ID_EQUALS(target, current)) continue;
            Vector2 target_pos = Vec(b2Body_GetPosition(target));
            Vector2 diff = Vector2Subtract(target_pos, GetPosition());
            diff = Vector2Normalize(diff);
        
            float angle_to_target = atan2(diff.y, diff.x);
            angle_to_target = Wrap(angle_to_target, -PI, PI);

            float delta = fabs(GetHeading() - angle_to_target);

            if (angle_to_target > GetHeading() && delta < min_delta) {
                min_delta = delta;
                min_angle_body = target;
            }
        }

        if (b2Body_IsValid(min_angle_body)) {
            SetLockedOnBody(min_angle_body);
        }
    }

    if (IsKeyPressed(KEY_Q) && b2Body_IsValid(GetLockedOnBody())) {
        b2BodyId current = GetLockedOnBody();
    
        float min_delta = std::numeric_limits<float>().max();
        b2BodyId min_angle_body = b2_nullBodyId;
    
        for (b2BodyId target : GetTargetsInRange()) {
            if (B2_ID_EQUALS(target, current)) continue;
            Vector2 target_pos = Vec(b2Body_GetPosition(target));
            Vector2 diff = Vector2Subtract(target_pos, GetPosition());
            diff = Vector2Normalize(diff);
        
            float angle_to_target = atan2(diff.y, diff.x);
            angle_to_target = Wrap(angle_to_target, -PI, PI);

            float delta = fabs(GetHeading() - angle_to_target);

            if (angle_to_target < GetHeading() && delta < min_delta) {
                min_delta = delta;
                min_angle_body = target;
            }
        }

        if (b2Body_IsValid(min_angle_body)) {
            SetLockedOnBody(min_angle_body);
        }
    }
}
