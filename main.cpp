#include <raylib.h>


int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(800, 600, "Topdown Game Time >:)");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
