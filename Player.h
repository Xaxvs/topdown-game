#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <box2d/box2d.h>

#include "Enemies.h"

#include "player_states.h"

constexpr long kDefaultAttackCooldown = 1;

class Player {
public:
  Player(b2WorldId world, int width, int height);
  ~Player() = default;
public:
  void Update();
  
  void Draw(Texture2D tilemap);
  
  void Attack(Enemies& enemies);

  void SetPosition(Vector2 position);
  Vector2 GetPosition() const;

  float GetHeading(void) const;
  void SetHeading(float heading);

  b2BodyId GetPlayerBody() const;
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

};

#endif // PLAYER_H
