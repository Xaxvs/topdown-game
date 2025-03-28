//
// Created by zinha on 3/28/2025.
//

#include "Pause.h"

Pause::Pause(nk_context* ctx) : context_(ctx) {

}

void Pause::SetPaused(const bool value) {
  paused_ = value;
}

bool Pause::IsPaused() const {
 return paused_;
}

void Pause::DrawPauseMenu(bool& menu, bool& draw) {
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  nk_text_colored(context_, "Paused", 20, NK_TEXT_CENTERED, ColorToNuklear(BLACK));
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  if (nk_button_label(context_, "Return")) {
    paused_ = !paused_;
  }
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  nk_spacer(context_);
  if(nk_button_label(context_, "Main Menu")) {
    paused_ = !paused_;
    menu = true;
    draw = true;
  }
}

