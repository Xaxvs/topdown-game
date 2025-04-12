#include "Enemies.h"

#include <raylib.h>


#include "Player.h"
#include "WindowManager.h"

#include <iostream>

static int fontSize = 25;

int main() {
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(800, 600, "Topdown Game Time >:)");
  nk_context *ctx = InitNuklear(fontSize);




  Enemies enemies;

  Player player("Player", 50, 50);
  player.SetX(250);
  player.SetY(250);

  enemies.AddEnemy({200, 100, 0, 0, 20, 50, 50, GREEN});

  Shop shop(ctx, player);

  auto windowManager = WindowManager(ctx, shop);

  Pause& pauseMenu = windowManager.GetPauseMenu();

  shop.AddItem({50, "Atk Incre", ShopItemType::INCREASE_ATTACK_DAMAGE});
  shop.AddItem({50, "Atk Incra", ShopItemType::INCREASE_ATTACK_DAMAGE});
  shop.AddItem({50, "Atk Incrs", ShopItemType::INCREASE_ATTACK_DAMAGE});
  shop.AddItem({50, "Atk Incre", ShopItemType::INCREASE_ATTACK_DAMAGE});
  shop.AddItem({50, "Atk Incry", ShopItemType::INCREASE_ATTACK_DAMAGE});
  shop.AddItem({50, "Atk Incru", ShopItemType::INCREASE_ATTACK_DAMAGE});
  shop.AddItem({50, "Atk Incri", ShopItemType::INCREASE_ATTACK_DAMAGE});
  shop.AddItem({50, "Atk Incro", ShopItemType::INCREASE_ATTACK_DAMAGE});

  while (!WindowShouldClose()) {
    windowManager.Update();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    int key = GetKeyPressed();

    if(key == KEY_P) {
      pauseMenu.SetPaused(!pauseMenu.IsPaused());
    }

    if(key == KEY_Q) {
      shop.setOpen(!shop.isOpen());
    }

    if(windowManager.DrawWindow() || pauseMenu.IsPaused() || shop.isOpen()) {
      DrawNuklear(ctx);
      EndDrawing();
      continue;
    }

    if(!pauseMenu.IsPaused() || !shop.isOpen()) {
      player.Attack(enemies);
      player.Update();
      enemies.Draw();
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
