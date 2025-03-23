#include <raylib.h>

int main() {
    InitWindow(1600, 1480, "Topdown Game Time >:)");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
