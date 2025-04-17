#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <box2d/box2d.h>

#include <vector>

#include "player_states.h"

constexpr long kDefaultAttackCooldown = 1;

class Player {
public:
  Player(b2WorldId world, int width, int height);
  ~Player() = default;
public:
  void Update();
  
  void Draw(Texture2D tilemap);
  
  void SetPosition(Vector2 position);
  Vector2 GetPosition() const;

  float GetHeading(void) const;
  void SetHeading(float heading);

  b2BodyId GetPlayerBody() const;

  void LockOn(b2WorldId world);

  void SetLockedOnBody(b2BodyId body);
  void PushLockedOnTarget(b2BodyId target);
  b2BodyId GetLockedOnBody();

  const std::vector<b2BodyId>& GetTargetsInRange() const;
private:
  PlayerStates states_;
  
  Rectangle player_texture_;
  Rectangle hand_texture_;
  Rectangle sword_texture_;

  b2BodyId player_body_;
  b2ShapeId player_shape_;
  
  float ray_angle_{};
    
  Vector2 position_;
  Vector2 velocity_;

  float target_angle_;
  float current_angle_;
  
  int width_, height_;
  float attack_cooldown_{};

  b2BodyId lock_on_target_;
  std::vector<b2BodyId> locked_on_targets_;
};

#endif // PLAYER_H
