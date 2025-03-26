#include "Enemies.h"

#include <raylib.h>

#include "MainMenu.h"
#include "Player.h"
#include "WindowManager.h"

int main() {
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(800, 600, "Topdown Game Time >:)");
  nk_context *ctx = InitNuklear(25);
  auto windowManager = WindowManager(ctx);

  Enemies enemies;

  Player player("Player", 50, 50);
  player.SetX(250);
  player.SetY(250);

  enemies.AddEnemy({200, 100, 0, 0, 20, 50, 50, GREEN});

  while (!WindowShouldClose()) {
    windowManager.Update();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if(windowManager.DrawWindow()) {
      DrawNuklear(ctx);
    }else {
      player.Update();
      player.Attack(enemies);

      enemies.Draw();
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
