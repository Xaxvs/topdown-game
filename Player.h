
#ifndef PLAYER_H
#define PLAYER_H
#include "Enemies.h"

#include <raylib.h>
#include <string>

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
  Item& GetWeapon();
  Item& GetShield();
  void SetWeapon(const Item& weapon);
  void SetShield(const Item& shield);
  void Attack(Enemies& enemies);

  void SetPosition(Vector2 position);
  Vector2 GetPosition() const;
private:
  float ray_angle_{};
  std::string display_name_;
  
  Item weapon_;
  Item shield_;
  
  Vector2 position_;
  Vector2 velocity_;
  
  int width_, height_;
  float attack_cooldown_{};

};

#endif // PLAYER_H
