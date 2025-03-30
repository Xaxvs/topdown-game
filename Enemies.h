//
// Created by zinha on 3/24/2025.
//

#ifndef ENEMIES_H
#define ENEMIES_H

#include <optional>
#include <raylib.h>
#include <vector>

struct Enemy {
  Vector2 position_;
  Vector2 velocity_;
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
