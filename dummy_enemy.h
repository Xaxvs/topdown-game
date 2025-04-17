#ifndef DUMMY_ENEMY_H_
#define DUMMY_ENEMY_H_

#include <box2d/box2d.h>

#include "enemy.h"

class DummyEnemy : public Enemy {
public:
  DummyEnemy(Vector2 position, b2WorldId world);
  ~DummyEnemy();
  
  void Update() override;
  void Draw() override;
  
  b2BodyId GetBody() const;
  b2ShapeId GetShape() const;
private:
  Texture2D sprite_;
  b2BodyId body_;
  b2ShapeId shape_;
};
#endif
