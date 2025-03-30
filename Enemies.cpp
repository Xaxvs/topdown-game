
#include "Enemies.h"

#include <cmath>

constexpr float kKnockbackForce = 5.0f;

std::vector<Enemy>& Enemies::GetEnemies() {
  return m_enemies;
}

void Enemies::AddEnemy(const Enemy& enemy) {
  m_enemies.push_back(enemy);
}

std::optional<Enemy*> Enemies::GetEnemyAt(const float x, const float y) {
  for(Enemy& enemy : GetEnemies()) {
    if (x >= enemy.position_.x && x <= enemy.position_.y + enemy.width_ &&
      y >= enemy.position_.x && y <= enemy.position_.y + enemy.height_) {
      return &enemy;
      }
  }
  return std::nullopt;
}

void Enemies::KillEnemy(const Enemy* enemy) {
  m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
      [enemy](const Enemy& e) { return &e == enemy; }),
      m_enemies.end());

  //death animation or something
}

void Enemies::AttackEnemy(Enemy* enemy, const int damage, float p_x, float p_y) {
  enemy->health_-=damage;

  //play sounds and animations
  float dx = enemy->position_.x - p_x;
  float dy = enemy->position_.y  - p_y;
  float angle = std::atan2(dy, dx);

  enemy->velocity_.x += std::cos(angle) * kKnockbackForce;
  enemy->velocity_.y += std::sin(angle) * kKnockbackForce;

  if(enemy->health_ <= 0) KillEnemy(enemy);
}

void Enemies::Draw() {
  for(Enemy& enemy : m_enemies) {

    enemy.position_.x += enemy.velocity_.x;
    enemy.position_.y += enemy.velocity_.y;

    enemy.velocity_.x *= 0.95f;
    enemy.velocity_.y *= 0.95f;

    if (std::fabs(enemy.velocity_.x) < 0.01f) enemy.velocity_.x = 0;
    if (std::fabs(enemy.velocity_.y) < 0.01f) enemy.velocity_.y = 0;

    std:std::pmr::string str = "HP: ";
    str.append(std::to_string(enemy.health_));

    DrawText(str.c_str(), enemy.position_.x + 10 , enemy.position_.y - 10, 10, BLACK);
    DrawRectangle(enemy.position_.x, enemy.position_.y, enemy.width_, enemy.height_, enemy.color_);
  }
}
