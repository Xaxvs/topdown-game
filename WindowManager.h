//
// Created by zinha on 3/26/2025.
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
#include "MainMenu.h"
#include "Pause.h"
#include "Shop.h"

class WindowManager {

public:
  explicit WindowManager(nk_context *ctx, Shop& shop) : ctx_(ctx), main_menu_(MainMenu(ctx, draw_window_)), pause_(Pause(ctx)), shop_(shop) {
    const Texture2D texture = LoadTexture("../assets/kenney_scribble-dungeons/PNG/Double (128px)/coffin.png");
    img_ = TextureToNuklear(texture);
    rect_ = {0, 0, 800, 600};
  }

  void Update() {
    if(!draw_window_ && !pause_.IsPaused() && !shop_.isOpen()) return;
    UpdateNuklear(ctx_);

    if (nk_begin(ctx_, "", nk_rect(0, 0, 800, 600), NK_FIXED)) {
      nk_layout_row_static(ctx_, 50, 250, 3);
      nk_command_buffer *buf = nk_window_get_canvas(ctx_);

      nk_draw_image(buf, rect_, &img_, ColorToNuklear(WHITE));
      if(draw_window_) main_menu_.Draw();
      if(pause_.IsPaused()) pause_.DrawPauseMenu(draw_window_, main_menu_.GetMenuValue());
      if(shop_.isOpen()) shop_.Draw();
    }
    nk_end(ctx_);

    BeginDrawing();
  }

  bool DrawWindow() const { return draw_window_; }

  Pause& GetPauseMenu() { return pause_; }
  Shop& GetShop() { return shop_; }

private:
  bool draw_window_ = true;
  nk_context *ctx_;
  struct nk_image img_ {};
  struct nk_rect rect_ {};
  MainMenu main_menu_;
  Pause pause_;
  Shop& shop_;
};

#endif // WINDOWMANAGER_H
