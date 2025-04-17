#include "enemy.h"

void Enemy::SetHealth(int health) {
  health_ = health;
}

int Enemy::GetHealth() const {
  return health_;
}

void Enemy::SetPosition(Vector2 position) {
  position_ = position;
}

Vector2 Enemy::GetPosition() const {
  return position_;
}


