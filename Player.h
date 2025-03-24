
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

  explicit Player(const std::string &display_name)
      : m_display_name(display_name),
        m_weapon({"Default Knife", 10, 5, ItemType::WEAPON}),
        m_shield({"Default Chestplate", 5, 5, ItemType::SHIELD}) {}

public:
  const std::string &GetDisplayName() const;
  void Update(KeyboardKey key_pressed);
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
  int m_x{}, m_y{};
};

#endif // PLAYER_H
