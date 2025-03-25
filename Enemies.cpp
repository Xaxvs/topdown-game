
#include "Enemies.h"

std::vector<Enemy>& Enemies::GetEnemies() {
  return m_enemies;
}

void Enemies::AddEnemy(const Enemy& enemy) {
  m_enemies.push_back(enemy);
}

std::optional<Enemy*> Enemies::GetEnemyAt(const float x, const float y) {
  for(Enemy& enemy : GetEnemies()) {
    if (x >= enemy.x_ && x <= enemy.x_ + enemy.width_ &&
      y >= enemy.y_ && y <= enemy.y_ + enemy.height_) {
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

void Enemies::AttackEnemy(Enemy* enemy, const int damage) {
  enemy->health_-=damage;

  //play sounds and animations, push enemy back using velocity += 10?
  // continuially

  if(enemy->health_ <= 0) KillEnemy(enemy);
}

void Enemies::Draw() {
  for(const auto &[x_, y_, health_, width_, height_, color_, speed_] : m_enemies) {

    std:std::pmr::string str = "HP: ";
    str.append(std::to_string(health_));

    DrawText(str.c_str(), x_ + 10 , y_ - 10, 10, BLACK);
    DrawRectangle(x_, y_, width_, height_, color_);
  }
}