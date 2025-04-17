#include "dummy_enemy.h"

#include "units.h"

DummyEnemy::DummyEnemy(Vector2 position, b2WorldId world) {
  SetPosition(position);
  
  sprite_ = LoadTexture("../assets/cat.png");

  b2BodyDef def = b2DefaultBodyDef();
  def.type = b2BodyType::b2_staticBody;
  def.position = b2Vec(GetPosition());

  body_ = b2CreateBody(world, &def);

  b2ShapeDef shape_def = b2DefaultShapeDef();  
  shape_def.filter.categoryBits |= 0x002;
  
  b2Polygon polygon = b2MakeBox(32.0f * kPixelToMeter, 32.0f * kPixelToMeter);
  shape_ = b2CreatePolygonShape(body_, &shape_def, &polygon);
}

DummyEnemy::~DummyEnemy() {
  UnloadTexture(sprite_);
}

void DummyEnemy::Update() {
  
}

void DummyEnemy::Draw() {
  Vector2 pos = GetPosition();
  pos.x -= 32;
  pos.y -= 32;
  DrawTextureEx(sprite_, pos, 0.0f, 0.5f, WHITE);
}

b2BodyId DummyEnemy::GetBody() const {
  return body_;
}

b2ShapeId DummyEnemy::GetShape() const {
  return shape_;
}
