#include "units.h"

Vector2 Vec(b2Vec2 vec) {
  return { vec.x * kMeterToPixel, vec.y * kMeterToPixel };
}

b2Vec2 b2Vec(Vector2 vec) {
  return { vec.x * kPixelToMeter, vec.y * kPixelToMeter };
}
