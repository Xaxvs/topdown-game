
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

  explicit Player(const std::string &display_name, int width, int height):
  display_name_(display_name),
  weapon_({"Default Knife", 10, 5, ItemType::WEAPON}),
  shield_({"Default Chestplate", 5, 5, ItemType::SHIELD}),
  width_(width),
  height_(height){}

public:
  const std::string &GetDisplayName() const;
  void Update();
  Item &GetWeapon();
  Item &GetShield();
  void SetWeapon(const Item &weapon);
  void SetShield(const Item &shield);
  int GetX() const;
  int GetY() const;
  int SetX(int value);
  int SetY(int value);
  void Attack(Enemies& enemies);

private:
  float ray_angle_{};
  const std::string &display_name_;
  Item weapon_;
  Item shield_;
  float x_{}, y_{}, vel_x_{}, vel_y_{};
  int width_, height_;
  float attack_cooldown_{};

};

#endif // PLAYER_H
