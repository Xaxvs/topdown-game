#ifndef PHYSICS_WORLD_H_
#define PHYSICS_WORLD_H_

#include <box2d/box2d.h>

#include <unordered_map>

#include "tile.h"

class PhysicsWorld {
public:  
  PhysicsWorld();
  ~PhysicsWorld();
  
  void UpdateWalls(const std::vector<Tile>& grid, int map_width);
  b2WorldId GetWorld() const;

  void Update(void) const;

  //Will add a wall if it doesn't already exist
  void AddWall(int index, int map_width);
  
  void DestroyWall(int index);
  bool WallExists(int index) const;
private:
  b2WorldId world_;
  std::unordered_map<int, b2BodyId> walls_;
};



#endif
