#ifndef UNITS_H_
#define UNITS_H_

#include <raylib.h>
#include <box2d/types.h>

constexpr float kMeterToPixel = 64.0f;
constexpr float kPixelToMeter = 1.0f / kMeterToPixel;

//Automatically converts from meters (box2d's units) to pixels (for rendering)
Vector2 Vec(b2Vec2 vec);

//Automatically converts from pixels to meters (for box2d)
b2Vec2 b2Vec(Vector2 vec);

#endif
