#include <raylib.h>
#include <raylib-nuklear.h>


int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Topdown Game Time >:)");

    struct nk_context* ctx = InitNuklear(12);

    struct nk_image nk_tilesheet = LoadNuklearImage("../assets/kenney_scribble-dungeons/Tilesheet/tilesheet.png");

    struct nk_vec2 selected = nk_vec2i(0, 0);
    

    while (!WindowShouldClose()) {

        UpdateNuklear(ctx);

        constexpr size_t kNumFramesHorizontal = 14;
        constexpr size_t kNumFramesVertical = 11;
        constexpr float kSpriteWidth = 63;
        constexpr float kSpriteHeight = 65;

        if (nk_begin(ctx, "Editor", nk_rect(10.0, 10.0, 896.0, 704.0), NK_WINDOW_MOVABLE | NK_WINDOW_TITLE)) {
            nk_layout_row_dynamic(ctx, 704, 1);
            nk_image(ctx, nk_tilesheet);

            struct nk_rect button = nk_rect(0, 0, 16, 16);          
            struct nk_rect content_region = nk_window_get_content_region(ctx);
            
            nk_uint offset_x = 0;
            nk_uint offset_y = 0;
            
            nk_window_get_scroll(ctx, &offset_x, &offset_y);
  
            struct nk_command_buffer* canvas = nk_window_get_canvas(ctx);
            for (int row = 0; row < kNumFramesHorizontal; ++row) {
              for (int col = 0; col < kNumFramesVertical; ++col) {
                struct nk_rect frame = nk_rect(row * kSpriteWidth, col * kSpriteHeight, kSpriteWidth, kSpriteHeight);
                frame.x += content_region.x - offset_x;
                frame.y += content_region.y - offset_y;
                
                bool mouse_over_window = nk_input_is_mouse_hovering_rect(&ctx->input, content_region);
                if (col == selected.x && row == selected.y) {
                  nk_stroke_rect(canvas, frame, 0.0, 5.0, nk_rgb(200, 255, 200));
                } else if (nk_input_is_mouse_hovering_rect(&ctx->input, frame) && mouse_over_window) {                   
                  if (nk_input_is_mouse_click_in_rect(&ctx->input, NK_BUTTON_LEFT, frame)) {
                    selected = nk_vec2(col, row);
                  }
                  nk_stroke_rect(canvas, frame, 0.0, 5.0, nk_rgb(200, 200, 255));
                } else {
                  nk_stroke_rect(canvas, frame, 0.0, 1.5, nk_rgb(250, 120, 120));
                }
                
              }
            }

        }
        nk_end(ctx);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawNuklear(ctx);

        EndDrawing();
    }

    UnloadNuklear(ctx);
    CloseWindow();
    
    return 0;
}
