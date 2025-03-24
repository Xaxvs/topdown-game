
#ifndef PLAYER_H
#define PLAYER_H
#include <raylib.h>
#include <string>

enum class ItemType { SHIELD, WEAPON };

struct Item {
  std::string name_;
  int damage_;
  int cost_;
  ItemType type_;
};

class Player {

public:

  explicit Player(const std::string &display_name, int width, int height)
      : m_display_name(display_name), m_width(width), m_height(height),
        m_weapon({"Default Knife", 10, 5, ItemType::WEAPON}),
        m_shield({"Default Chestplate", 5, 5, ItemType::SHIELD}) {}

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
  void attack() const;

private:
  const std::string &m_display_name;
  Item m_weapon;
  Item m_shield;
  float m_x{}, m_y{}, m_velX, m_velY;
  int m_width, m_height;
};

#endif // PLAYER_H
