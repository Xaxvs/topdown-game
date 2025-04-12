#ifndef SHOP_H
#define SHOP_H
#include "Player.h"

#include <raylib-nuklear.h>
#include <string>
#include <vector>

enum class ShopItemType {
  INCREASE_DEFENSE,
  INCREASE_ATTACK_DAMAGE,
  INCREASE_SPEED,
  INCREASE_HEALTH,
  DECREASE_ATTACK_COOLDOWN,
} typedef ShopItemType;

struct ShopItem {
  int cost_;
  std::string name_;
  ShopItemType type_;
  int itemTypeValue_;
  // TODO anything else you want to add
  bool operator==(const ShopItem& other) const {
    return cost_ == other.cost_ && name_ == other.name_ && itemTypeValue_ == other.itemTypeValue_;
  }
};

class Shop {

public:
  Shop(nk_context* ctx, Player& player) : ctx_(ctx), player_(player) {
  }
  void Draw();
  bool AddItem(const ShopItem& item);
  bool PurchaseItem(const ShopItem& item, int& playerMoney);
  std::vector<ShopItem>& GetItems();
  bool isOpen() const;
  void setOpen(bool value);
  const Player& getPlayer() const;

private:
  nk_context* ctx_;
  std::vector<ShopItem> items_{};
  bool isOpen_ = false;
  Player& player_;

};

#endif
