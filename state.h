#ifndef STATE_H_
#define STATE_H_

#include <raymath.h>

class State {
public:
  State() = default;
  virtual ~State() {};
  
  virtual void OnEnter() {};
  virtual void OnUpdate() {};
  virtual void OnDraw(Vector2 position, Vector2 origin, Vector2 size, float rotation) {};
  virtual void OnExit() {};
};
#endif
