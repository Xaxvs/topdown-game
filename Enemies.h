//
// Created by zinha on 3/24/2025.
//

#ifndef ENEMIES_H
#define ENEMIES_H
#include <algorithm>
#include <optional>
#include <raylib.h>
#include <vector>

struct Enemy {
  float x_, y_;
  int health_;
  float width_, height_;
  Color color_;
  float speed_;

};

class Enemies {

public:
  Enemies() : m_enemies(std::vector<Enemy>()) {};

  const std::vector<Enemy>& getEnemies() const {
    return m_enemies;
  }

  void addEnemy(const Enemy& enemy) {
    m_enemies.push_back(enemy);
  }

  std::optional<const Enemy*> getEnemyAt(const float x, const float y) const {
    for(const Enemy& enemy : getEnemies()) {
      if (x >= enemy.x_ && x <= enemy.x_ + enemy.width_ &&
        y >= enemy.y_ && y <= enemy.y_ + enemy.height_) {
        return &enemy;
        }
    }
    return std::nullopt;
  }

  void kill(const Enemy* enemy) {
    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
        [enemy](const Enemy& e) { return &e == enemy; }),
        m_enemies.end());

    //death animation or something
  }

  void draw() {
    for(const auto &[x_, y_, health_, width_, height_, color_, speed_] : m_enemies) {
      DrawText("HP: " + health_, x_ + width_ / 2 , y_ - 10, 10, BLACK);
      DrawRectangle(x_, y_, width_, height_, color_);
    }
  }

private:
  std::vector<Enemy> m_enemies{};
};

#endif //ENEMIES_H
