//
// Created by zinha on 3/28/2025.
//

#ifndef PAUSE_H
#define PAUSE_H
#include <raylib-nuklear.h>



class Pause {
public:
  explicit Pause(nk_context* ctx);
  bool IsPaused() const;
  void SetPaused(bool value);
  void DrawPauseMenu(bool& menu, bool& draw);

private:
  bool paused_ = false;
  nk_context* context_;
};



#endif //PAUSE_H
