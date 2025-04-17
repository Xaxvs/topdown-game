#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>

class Enemy {
public:
  Enemy() = default;
  virtual ~Enemy() = default;
  
  virtual void Start() {};
  virtual void Update() {};
  virtual void Draw() {};

  void SetHealth(int health);
  int GetHealth() const;

  void SetPosition(Vector2 position);
  Vector2 GetPosition() const;
private:
  int health_;
  Vector2 position_;
};

#endif //ENEMY_H
