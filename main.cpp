#include <raylib.h>
#include <raylib-nuklear.h>

#include <vector>

#include "tile.h"
#include "editor.h"
#include "physics_world.h"

#include "units.h"

#include "dummy_enemy.h"

#include "Player.h"

enum class Mode {
  kModeEditor,
  kModeGame  
};

int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Topdown Game Time >:)");

    SetTargetFPS(60);

    struct nk_context* ctx = InitNuklear(18);

    Texture2D tilemap = LoadTexture("../assets/kenney_scribble-dungeons/Tilesheet/tilesheet.png");
    SetExitKey(-1);
    
    PhysicsWorld physics;
        
    LevelEditor editor(&physics);
    
    std::vector<Tile> grid;
    grid.resize(1);
    
    editor.Load("bettertest.txt", grid);
    int grid_width = editor.GetGridWidth();
    int grid_height = editor.GetGridHeight();
    
    Player player = Player(physics.GetWorld(), 64, 64);
        
    Mode current = Mode::kModeEditor;

    Camera2D game_camera;
    game_camera.zoom = 5.0f;
    game_camera.target = player.GetPosition();
    game_camera.rotation = 0.0f;
    game_camera.offset = { 400.0f, 400.0f };

    DummyEnemy dummy({ 192 + 32, 32 }, physics.GetWorld());
    
    while (!WindowShouldClose()) {
        UpdateNuklear(ctx);

        physics.Update();

        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Q)) {
            if (current == Mode::kModeEditor) {
                current = Mode::kModeGame;
                editor.SetMode(EditMode::kNone);
                
                b2BodyId player_body = player.GetPlayerBody();
                b2Body_SetTransform(player_body, b2Vec2 { 0.0f, 0.0f }, b2Rot_identity);
                player.SetPosition(Vec(b2Vec2 { 0.0, 0.0 }));
                player.SetHeading(0.0f);
            } else {
                current = Mode::kModeEditor;
                
                b2BodyId player_body = player.GetPlayerBody();
                b2Body_SetTransform(player_body, b2Vec2 { -1.0f, -1.0f }, b2Rot_identity);
                player.SetPosition(Vec(b2Vec2 { -1.0, -1.0 }));
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        switch (current) {
            case Mode::kModeEditor: {   
                editor.UpdateEditor(grid);
                
                BeginMode2D(editor.GetCamera());
                editor.DrawTileMap(grid, tilemap);        
                EndMode2D();        
                
                editor.DrawUI(ctx);
                DrawNuklear(ctx);
                break;
            }
            case Mode::kModeGame: {                    

                if (IsKeyPressed(KEY_TAB)) {
                    if (b2Body_IsValid(player.GetLockedOnBody())) {
                        player.SetLockedOnBody(b2_nullBodyId);
                    } else {
                        player.LockOn(physics.GetWorld());
                    }
                }
                
                player.Update();
                
                float screenHeight = GetScreenHeight();
                float screenWidth = GetScreenWidth();
                
                if (b2Body_IsValid(player.GetLockedOnBody())) {
                    Vector2 locked_pos = Vec(b2Body_GetPosition(player.GetLockedOnBody()));
                    Vector2 dist = Vector2Subtract(locked_pos, player.GetPosition());
                    dist = Vector2Add(player.GetPosition(), Vector2Scale(dist, 0.5f));
                    game_camera.target = Vector2Lerp(game_camera.target, dist, 0.05f);
                } else {
                    game_camera.target = Vector2Lerp(game_camera.target, player.GetPosition(), 0.5f);
                }
                
                game_camera.offset = Vector2 { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
                game_camera.offset.x *= GetWindowScaleDPI().x;
                game_camera.offset.y *= GetWindowScaleDPI().y;

                                
                BeginMode2D(game_camera);
                editor.DrawTileMap(grid, tilemap);        

                player.Draw(tilemap);

                dummy.Draw();
                
                b2BodyId locked_on = player.GetLockedOnBody();
                if (b2Body_IsValid(locked_on)) {
                    Vector2 locked_pos = Vec(b2Body_GetPosition(locked_on));
                    DrawCircle(locked_pos.x, locked_pos.y, 2.0f, WHITE);
                }

                EndMode2D();        
                                
                break;
            }
        }

        EndDrawing();
    }
    
    UnloadTexture(tilemap);
    UnloadNuklear(ctx);
    CloseWindow();
    
    return 0;
}
