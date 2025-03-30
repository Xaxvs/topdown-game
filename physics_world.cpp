#include "physics_world.h"

#include <vector>

#include "editor.h"
#include "units.h"

PhysicsWorld::PhysicsWorld() {
  b2WorldDef world_def = b2DefaultWorldDef();
  world_def.gravity = b2Vec2_zero;

  world_ = b2CreateWorld(&world_def);
}

PhysicsWorld::~PhysicsWorld() {
  b2DestroyWorld(world_);
}

void PhysicsWorld::UpdateWalls(const std::vector<Tile>& grid, int map_width) {
  for (int i = 0; i < grid.size(); ++i) {
    Tile tile = grid[i];
    
    if (WallExists(i) && !tile.is_wall_) {
      DestroyWall(i);
      continue;
    } else if (WallExists(i) || !tile.is_wall_) continue;

    int x = i % map_width;
    int y = i / map_width;

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.position = b2Vec2 { (float)x + 0.5f, (float)y + 0.5f };
    b2BodyId wall = b2CreateBody(world_, &body_def);

    float size = kGridSize * kPixelToMeter * 0.5f;
    
    b2Polygon wall_box = b2MakeSquare(size);
    b2ShapeDef shape_def = b2DefaultShapeDef();
    b2CreatePolygonShape(wall, &shape_def, &wall_box);

    walls_.insert(std::make_pair(i, wall));
  }
}

void PhysicsWorld::DestroyWall(int index) {
  if (!WallExists(index)) return;
  b2DestroyBody(walls_.at(index));
  walls_.erase(index);
}

void PhysicsWorld::Update(void) const {
  b2World_Step(world_, 1.0f / 60.0f, 4);
}
  
bool PhysicsWorld::WallExists(int index) const {
  return walls_.find(index) != walls_.cend();
}

b2WorldId PhysicsWorld::GetWorld() const {
  return world_;
}
