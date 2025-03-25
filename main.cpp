#include <raylib.h>
#include <raylib-nuklear.h>

#include <vector>

#include "tile.h"
#include "editor.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Topdown Game Time >:)");

    struct nk_context* ctx = InitNuklear(18);

    LevelEditor editor;

    std::vector<Tile> grid;
    grid.resize(1);

    Texture2D tilemap = LoadTexture("../assets/kenney_scribble-dungeons/Tilesheet/tilesheet.png");
    SetExitKey(-1);


    while (!WindowShouldClose()) {
        UpdateNuklear(ctx);

        editor.UpdateEditor(grid);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode2D(editor.GetCamera());
        editor.DrawTileMap(grid, tilemap);        
        
        EndMode2D();
        
        editor.DrawUI(ctx);
        
        DrawNuklear(ctx);

        EndDrawing();
    }

    UnloadTexture(tilemap);
    UnloadNuklear(ctx);
    CloseWindow();
    
    return 0;
}
