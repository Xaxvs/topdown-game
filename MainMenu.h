
#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include <iostream>
#include <raylib-nuklear.h>

#include "Button.h"

class MainMenu {

public:
  MainMenu() = default;

  void Draw(nk_context *ctx) {
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_text(ctx, "Topdown-Game", 13, NK_TEXT_ALIGN_CENTERED);
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    if (nk_button_label(ctx, "PLAY"))
      OnPlayClick();
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    if (nk_button_label(ctx, "SETTINGS"))
      OnSettingsClick();
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    nk_spacer(ctx);
    if (nk_button_label(ctx, "CREDITS"))
      OnCreditsClick();
  }

private:
  void OnCreditsClick() { std::cout << "CREDITS"; }

  void OnPlayClick() { std::cout << "PLAY"; }

  void OnSettingsClick() { std::cout << "SETTINGS"; }
};

#endif // MAIN_MENU_H
