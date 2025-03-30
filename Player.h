#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <box2d/box2d.h>

#include <string>

#include "Enemies.h"

constexpr long kDefaultAttackCooldown = 1;

enum class ItemType { SHIELD, WEAPON };

struct Item {
  std::string name_;
  int damage_;
  int cost_;
  ItemType type_;
};

class Player {
public:
  Player(const std::string &display_name, int width, int height);
public:
  const std::string& GetDisplayName() const;
  void Update();
  
  void Draw(Texture2D tilemap);
  
  Item& GetWeapon();
  Item& GetShield();
  void SetWeapon(const Item& weapon);
  void SetShield(const Item& shield);
  void Attack(Enemies& enemies);

  void SetPosition(Vector2 position);
  Vector2 GetPosition() const;

  float GetHeading(void) const;
private:
  Rectangle player_texture_;
  Rectangle hand_texture_;

  
  float ray_angle_{};
  std::string display_name_;
  
  Item weapon_;
  Item shield_;
  
  Vector2 position_;
  Vector2 velocity_;

  float target_angle_;
  
  int width_, height_;
  float attack_cooldown_{};

};

#endif // PLAYER_H
