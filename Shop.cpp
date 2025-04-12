
#include "Shop.h"

void Shop::Draw() {
  nk_label(ctx_, "Shop", NK_TEXT_CENTERED);
  nk_spacer(ctx_);
  nk_spacer(ctx_);
  nk_spacer(ctx_);
  nk_spacer(ctx_);
  for(const ShopItem& item : items_) {
    if(nk_button_label(ctx_, (item.name_ + " Cost: " + std::to_string(item.cost_)).c_str())) {
      PurchaseItem(item, player_.GetMoney());
    }
    nk_spacer(ctx_);
  }
}

const Player &Shop::getPlayer() const {
  return player_;
}

bool Shop::isOpen() const {
  return isOpen_;
}

void Shop::setOpen(const bool value) {
  isOpen_ = value;
}

std::vector<ShopItem>& Shop::GetItems() {
  return items_;
}

bool Shop::AddItem(const ShopItem& item) {
  /*for(const ShopItem& shopItem : items_) {
    if(shopItem == item) {
      return false;
    }
  }*/
  items_.push_back(item);
  return true;
}


bool Shop::PurchaseItem(const ShopItem& item, int& playerMoney) {
  if(playerMoney >= item.cost_) {
    playerMoney -= item.cost_;
    if (const auto it = std::find(items_.begin(), items_.end(), item);
        it != items_.end()) {
      items_.erase(it);
    }
    switch (item.type_) {
      case ShopItemType::INCREASE_SPEED:
        //speed
        break;
      case ShopItemType::INCREASE_HEALTH:
        //health
        break;
      case ShopItemType::INCREASE_ATTACK_DAMAGE:
        //atk damage
        break;
      case ShopItemType::INCREASE_DEFENSE:
        //inc defense
        break;
      case ShopItemType::DECREASE_ATTACK_COOLDOWN:
        //decrease atk cooldown
        break;
    }
    return true;
  }
  return false;
}

