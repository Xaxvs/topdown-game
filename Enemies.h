//
// Created by zinha on 3/24/2025.
//

#ifndef ENEMIES_H
#define ENEMIES_H

#include <algorithm>
#include <optional>
#include <raylib.h>
#include <regex>
#include <string>
#include <vector>

struct Enemy {
  float x_, y_, vel_x_, vel_y_;
  int health_;
  float width_, height_;
  Color color_;
  float speed_;
};

class Enemies {

public:
  Enemies() : m_enemies(std::vector<Enemy>()) {};

  std::vector<Enemy>& GetEnemies();

  void AddEnemy(const Enemy& enemy);

  std::optional<Enemy*> GetEnemyAt(float x, float y);

  void KillEnemy(const Enemy* enemy);

  void AttackEnemy(Enemy* enemy, int damage, float p_x, float p_y);

  void Draw();

private:
  std::vector<Enemy> m_enemies{};
};

#endif //ENEMIES_H
