#include <raylib.h>

#include "MainMenu.h"


int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(800, 600, "Topdown Game Time >:)");
    nk_context* ctx = InitNuklear(25);
    auto menu = MainMenu();
    const Texture2D texture = LoadTexture("../assets/kenney_scribble-dungeons/PNG/Double (128px)/coffin.png");
    const struct nk_image img = TextureToNuklear(texture);
    const struct nk_rect rect{0, 0, 800 ,600};


    while (!WindowShouldClose()) {
        UpdateNuklear(ctx);

        if(nk_begin(ctx, "", nk_rect(0, 0, 800, 600),NK_FIXED)) {
            nk_layout_row_static(ctx, 50, 250, 3);

            nk_command_buffer* buf = nk_window_get_canvas(ctx);

            nk_draw_image(buf, rect, &img, ColorToNuklear(WHITE));

            menu.Draw(ctx);
        }
        nk_end(ctx);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawNuklear(ctx);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
