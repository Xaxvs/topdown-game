
#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include <iostream>
#include <utility>
#include <raylib-nuklear.h>

#include "Button.h"

class MainMenu {

public:
  MainMenu(nk_context *ctx, bool& draw_window) : ctx_(ctx), draw_window_(draw_window) {}

  void Draw() {
    if(!draw_window_) return;
    if(menu_enabled_) DrawMainMenu();
    if(settings_enabled) DrawSettings();
    if(credits_enabled_) DrawCredits();
  }

private:

  void DrawMainMenu() {
    DrawSpacer(4);
    nk_text(ctx_, "Topdown-Game", 13, NK_TEXT_ALIGN_CENTERED);
    DrawSpacer(5);
    if (nk_button_label(ctx_, "PLAY")) {
      OnPlayClick();
      draw_window_ = false;
    }
    DrawSpacer(5);
    if (nk_button_label(ctx_, "SETTINGS"))
      OnSettingsClick();
    DrawSpacer(5);
    if (nk_button_label(ctx_, "CREDITS"))
      OnCreditsClick();
  }

  void DrawSettings() {
    DrawSpacer(4);
    nk_text_colored(ctx_, "Settings", 13, NK_TEXT_ALIGN_CENTERED, ColorToNuklear(BLACK));
    DrawSpacer(5);
    nk_text_colored(ctx_, "No settings for you >:D", 20, NK_TEXT_ALIGN_LEFT, ColorToNuklear(BLACK));
    DrawSpacer(16);
    if (nk_button_label(ctx_, "BACK TO MENU"))
      BackToMenuClick(settings_enabled);
  }

  void DrawCredits() {
    DrawSpacer(4);
    nk_text_colored(ctx_, "Credits", 13, NK_TEXT_ALIGN_CENTERED, ColorToNuklear(BLACK));
    DrawSpacer(5);
    nk_text_colored(ctx_, "Xaxis and Logos and Pigon", 20, NK_TEXT_ALIGN_LEFT, ColorToNuklear(BLACK));
    DrawSpacer(16);
    if (nk_button_label(ctx_, "BACK TO MENU"))
      BackToMenuClick(credits_enabled_);
  }

  void DrawSpacer(const int amount) const {
    for(int i = 0; i < amount; i++) {
      nk_spacer(ctx_);
    }
  }

  void OnCreditsClick() {
    menu_enabled_ = false;
    credits_enabled_ = true;
  }

  void OnPlayClick() {
    draw_window_ = false;
    menu_enabled_ = false;
  }

  void OnSettingsClick() {
    menu_enabled_ = false;
    settings_enabled = true;
  }

  void BackToMenuClick(bool& menu) {
    menu = false;
    menu_enabled_ = true;
    draw_window_ = true;
  }

private:
  bool menu_enabled_ = true;
  bool credits_enabled_ = false;
  bool settings_enabled = false;
  nk_context* ctx_;
  bool& draw_window_;

};

#endif // MAIN_MENU_H
