#include <raylib.h>
#include <raylib-nuklear.h>

#include <vector>

#include "Tile.h"
#include "editor.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Topdown Game Time >:)");

    struct nk_context* ctx = InitNuklear(12);

    LevelEditor editor;

    std::vector<Tile> grid;
    grid.resize(1);

    Texture2D tilemap = LoadTexture("../assets/kenney_scribble-dungeons/Tilesheet/tilesheet.png");
    SetExitKey(-1);

    while (!WindowShouldClose()) {
        UpdateNuklear(ctx);

        if (editor.GetCurrentMode() == EditMode::kNone) {
          editor.UpdateCamera();
        }

        if (IsKeyPressed(KEY_E)) {
          editor.SetMode(EditMode::kTileSelect);
        } else if (IsKeyPressed(KEY_R)) {
          editor.SetMode(EditMode::kGridEdit);
        } else if (IsKeyPressed(KEY_ESCAPE)) {
          editor.SetMode(EditMode::kNone);
        }

        if (editor.GetCurrentMode() == EditMode::kGridEdit) {
          grid.resize(editor.GetGridWidth() * editor.GetGridHeight());
        }
        
        editor.Draw(ctx);

        
        BeginDrawing();
        
        BeginMode2D(editor.GetCamera());
        ClearBackground(RAYWHITE);
        for (int i = 0; i < grid.size(); ++i) {
          int column = i % editor.GetGridWidth();
          int row = i / editor.GetGridWidth();

          Tile tile = grid[i];
          int pos_x = column * 64;
          int pos_y = row * 64;

          Rectangle texture_rect;
          texture_rect.x = tile.texture_col_;
          texture_rect.y = tile.texture_row_;
          texture_rect.width = 64;
          texture_rect.height = 64;

          DrawTextureRec(tilemap, texture_rect, Vector2 { (float)pos_x, (float)pos_y }, WHITE);
        }
        EndMode2D();

        DrawNuklear(ctx);

        EndDrawing();
    }

    UnloadTexture(tilemap);
    
    UnloadNuklear(ctx);
    CloseWindow();
    
    return 0;
}
