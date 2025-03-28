#include "Enemies.h"

#include <raylib.h>


#include "Player.h"
#include "WindowManager.h"

static int fontSize = 25;

int main() {
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(800, 600, "Topdown Game Time >:)");
  nk_context *ctx = InitNuklear(fontSize);
  auto windowManager = WindowManager(ctx);

  Enemies enemies;

  Player player("Player", 50, 50);
  player.SetX(250);
  player.SetY(250);

  enemies.AddEnemy({200, 100, 0, 0, 20, 50, 50, GREEN});

  Pause& pauseMenu = windowManager.GetPauseMenu();

  while (!WindowShouldClose()) {
    windowManager.Update();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if(GetKeyPressed() == KEY_P) {
      pauseMenu.SetPaused(!pauseMenu.IsPaused());
    }

    if(windowManager.DrawWindow() || windowManager.GetPauseMenu().IsPaused()) {
      DrawNuklear(ctx);
      EndDrawing();
      continue;
    }

    if(!pauseMenu.IsPaused()) {
      player.Attack(enemies);
      player.Update();
      enemies.Draw();
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
