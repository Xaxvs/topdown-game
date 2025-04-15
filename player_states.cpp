#include "player_states.h"

#include <box2d/box2d.h>

#include "Player.h"
#include "units.h"

PlayerStates::PlayerStates(Player* player)
  : default_(Animation("../assets/SpriteSheetRedGuy.png", 19, 2, 38))
{
  idle_.manager_ = this;
  attack_.manager_ = this;
  attack2_.manager_ = this;
  attack3_.manager_ = this;

  idle_.player_ = player;
  attack_.player_ = player;
  attack2_.player_ = player;
  attack3_.player_ = player;
  
  current_state_ = &idle_;
  current_state_type_ = IState::kIStateIdle;
  current_state_->OnEnter();
}

void PlayerStates::Switch(PlayerStates::IState state) {
  if (current_state_type_ == state) return;

  current_state_type_ = state;
  
  if (current_state_ != nullptr) {
    current_state_->OnExit();
  }

  switch (state) {
    case IState::kIAttackState:
      current_state_ = &attack_;
      break;
    case IState::kIStateIdle:
      current_state_ = &idle_;
      break;
    case IState::kIAttack2State:
      current_state_ = &attack2_;
      break;
    case IState::kIAttack3State:
      current_state_ = &attack3_;
      break;
  }

  current_state_->OnEnter();
}

void PlayerStates::Update() {
  if (current_state_ == nullptr) return;
  current_state_->OnUpdate();
}

void PlayerStates::Draw(Vector2 position, Vector2 origin, Vector2 size, float rotation) {
  if (current_state_ == nullptr) return;
  current_state_->OnDraw(position, origin, size, rotation);
}

Animation& PlayerStates::GetAttackAnimation() {
  return default_;
}

PlayerStates::IState PlayerStates::GetCurrentState() const {
  return current_state_type_;
}

void IdleState::OnEnter() {
  Animation& anim = manager_->GetAttackAnimation();
  anim.SetFrameStart(0);
  anim.SetFrameEnd(0);
}

void IdleState::OnUpdate() {

  Animation& anim = manager_->GetAttackAnimation();
  anim.Update();
  
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    manager_->Switch(PlayerStates::IState::kIAttackState);
  }
  
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

  float current_angle = player_->GetHeading();
    
  float delta = target_angle_ - current_angle;
  if (delta > PI) {
    current_angle += 2 * PI;
  } else if (delta < -PI) {
    current_angle -= 2 * PI;
  }

  current_angle = Lerp(current_angle, target_angle_, 0.1);
  current_angle = Wrap(current_angle, -PI, PI);

  player_->SetHeading(current_angle);

  b2BodyId player_body = player_->GetPlayerBody();
    
  b2Vec2 current_velocity = b2Body_GetLinearVelocity(player_body);
  if (input_length > 0.f) {
    float speed = 2.0f;
    b2Vec2 new_velocity = b2Lerp(current_velocity, b2MulSV(speed, input), 0.1);
    b2Body_SetLinearVelocity(player_body, new_velocity);
  } else {
    b2Vec2 new_velocity = b2Lerp(current_velocity, b2Vec2_zero, 0.6);
    b2Body_SetLinearVelocity(player_body, new_velocity);
  }
}

void IdleState::OnDraw(Vector2 position, Vector2 origin, Vector2 size, float rotation) {
  Animation& anim = manager_->GetAttackAnimation();
  anim.Draw(position, origin, size, rotation);
}

void IdleState::OnExit() {
  b2BodyId player_body = player_->GetPlayerBody();
  b2Body_SetLinearVelocity(player_body, b2Vec2_zero);
}

void AttackState::OnEnter() {
  Animation& anim = manager_->GetAttackAnimation();
  anim.SetFrameStart(0);
  anim.SetFrameEnd(3);
  attack2_ = false;
}

void AttackState::OnUpdate() {
  Animation& anim = manager_->GetAttackAnimation();
  anim.Update();
  
  if (anim.GetCurrentFrame() == 8) {
    b2BodyId id = player_->GetPlayerBody();
    
    float heading = player_->GetHeading();
    
    Vector2 direction = { cosf(heading), sinf(heading) };
    direction = Vector2Normalize(direction);
    direction = Vector2Scale(direction, 2.5f);
    
    b2Body_ApplyLinearImpulseToCenter(id, b2Vec(direction), true);
  }

  if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && anim.GetCurrentFrame() < 3) {
    anim.SetFrameStart(3);
    anim.SetFrameEnd(5);

    b2BodyId id = player_->GetPlayerBody();
    b2Body_SetLinearVelocity(id, b2Vec2_zero);
    
  } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && anim.GetCurrentFrame() <= 5) {
    anim.SetFrameStart(5);
    anim.SetFrameEnd(16);
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && anim.GetCurrentFrame() >= 7) {
    attack2_ = true;
  }
  
  if (anim.GetCurrentFrame() >= 16 && !attack2_) {
    manager_->Switch(PlayerStates::IState::kIStateIdle);
  } else if (anim.GetCurrentFrame() >= 16 && attack2_) {
    manager_->Switch(PlayerStates::IState::kIAttack2State);
  }
}

void AttackState::OnDraw(Vector2 position, Vector2 origin, Vector2 size, float rotation) {
  Animation& anim = manager_->GetAttackAnimation();
  anim.Draw(position, origin, size, rotation);
}

void AttackState2::OnEnter() {
  Animation& anim = manager_->GetAttackAnimation();
  anim.SetFrameStart(18);
  anim.SetFrameEnd(24);

  attack3_ = false;
}

void AttackState2::OnUpdate() {
  Animation& anim = manager_->GetAttackAnimation();
  anim.Update();

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    attack3_ = true;
  }
    
  if (anim.GetCurrentFrame() >= anim.GetFrameEnd() && !attack3_) {
    manager_->Switch(PlayerStates::IState::kIStateIdle);
  } else if (anim.GetCurrentFrame() >= anim.GetFrameEnd() && attack3_){
    manager_->Switch(PlayerStates::IState::kIAttack3State);
  }
}

void AttackState2::OnDraw(Vector2 position, Vector2 origin, Vector2 size, float rotation) {
  Animation& anim = manager_->GetAttackAnimation();
  anim.Draw(position, origin, size, rotation);  
}

void AttackState3::OnEnter() {
  Animation& anim = manager_->GetAttackAnimation();
  anim.SetFrameStart(24);
  anim.SetFrameEnd(37);
}

void AttackState3::OnUpdate() {
  Animation& anim = manager_->GetAttackAnimation();
  anim.Update();
  
  if (anim.GetCurrentFrame() >= anim.GetFrameEnd()) {
    manager_->Switch(PlayerStates::IState::kIStateIdle);
  }
}

void AttackState3::OnDraw(Vector2 position, Vector2 origin, Vector2 size, float rotation) {
  Animation& anim = manager_->GetAttackAnimation();
  anim.Draw(position, origin, size, rotation);  
}
