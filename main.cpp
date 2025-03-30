#include <raylib.h>
#include <raylib-nuklear.h>

#include <box2d/box2d.h>
#include <box2d/math_functions.h>

#include <vector>


#include "tile.h"
#include "editor.h"
#include "units.h"
#include "Player.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Topdown Game Time >:)");

    SetTargetFPS(60);

    struct nk_context* ctx = InitNuklear(18);

    LevelEditor editor;

    std::vector<Tile> grid;
    grid.resize(1);

    Texture2D tilemap = LoadTexture("../assets/kenney_scribble-dungeons/Tilesheet/tilesheet.png");
    SetExitKey(-1);
    
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = b2Vec2_zero;

    b2WorldId world = b2CreateWorld(&world_def);
    
    Player player(world, "Steve", 64, 64);

    editor.Load("dummy2.txt", grid);
    int grid_width = editor.GetGridWidth();
    int grid_height = editor.GetGridHeight();

    std::vector<b2BodyId> bodies;
    std::vector<std::tuple<b2ShapeId, b2BodyId>> balls;
    
    for (int i = 0; i < grid.size(); ++i) {
        int x = i % grid_width;
        int y = i / grid_width;

        Tile tile = grid[i];
        if (!tile.is_wall_) continue;
        
        b2BodyDef body_def = b2DefaultBodyDef();
        body_def.position = b2Vec2 { (float)x + 0.5f, (float)y + 0.5f };
        b2BodyId wall = b2CreateBody(world, &body_def);

        float size = kGridSize * kPixelToMeter * 0.5f;
        
        b2Polygon wall_box = b2MakeSquare(size);
        b2ShapeDef shape_def = b2DefaultShapeDef();
        b2CreatePolygonShape(wall, &shape_def, &wall_box);

        bodies.push_back(wall);
    }

    float radius = 10.0f;

    while (!WindowShouldClose()) {
        UpdateNuklear(ctx);

        b2World_Step(world, 1.0 / 60.0, 4);

        editor.UpdateEditor(grid);
        if (IsKeyDown(KEY_SPACE)) {
            float angle = player.GetHeading();
            b2Vec2 dir { cosf(angle), sinf(angle) };
            dir = b2Normalize(dir);

            Vector2 player_pos = player.GetPosition();
            b2Vec2 pos = b2Vec(player_pos);
        
            b2BodyDef body_def = b2DefaultBodyDef();
            body_def.type = b2BodyType::b2_dynamicBody;
            body_def.position = b2Add(pos, b2MulSV(kPixelToMeter, dir));

            b2BodyId body = b2CreateBody(world, &body_def);

            b2ShapeDef shape_def = b2DefaultShapeDef();
            shape_def.density = 0.1;
            shape_def.restitution = 0.2;
            shape_def.friction = 0.0;

            radius = (float)GetRandomValue(5, 20);

            b2Circle circle { b2Vec2_zero, radius * kPixelToMeter };

            b2ShapeId shape = b2CreateCircleShape(body, &shape_def, &circle);

            b2Body_ApplyLinearImpulseToCenter(body, b2MulSV(kPixelToMeter * 2.0f, dir), true);
            
            balls.push_back(std::make_tuple(shape, body));
        }

        player.Update();
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode2D(editor.GetCamera());
        // editor.DrawTileMap(grid, tilemap);        

        for (int i = 0; i < grid.size(); ++i) {
            Tile tile = grid[i];
            int x = i % grid_width;
            int y = i / grid_width;

            Rectangle source {
                (float)tile.texture_col_ * kGridSize,
                (float)tile.texture_row_ * kGridSize,
                kGridSize,
                kGridSize
            };

            Rectangle dest {
               (float)x * kGridSize,
               (float)y * kGridSize,
               kGridSize,
               kGridSize
            };

            DrawTexturePro(tilemap, source, dest, Vector2 { 0.0, 0.0 }, 0.0f, WHITE);
        }

        player.Draw(tilemap);
        
        for (auto& [shape, body]: balls) {
            b2Vec2 pos = b2Body_GetPosition(body);
            float r = b2Shape_GetCircle(shape).radius * kMeterToPixel;
            DrawCircle(pos.x * kMeterToPixel, pos.y * kMeterToPixel, r, ColorBrightness(RED, r / 25.0));
            DrawCircleLines(pos.x * kMeterToPixel, pos.y * kMeterToPixel, r, BLACK);
        }
        
        EndMode2D();        
        
        editor.DrawUI(ctx);
        DrawNuklear(ctx);

        EndDrawing();
    }
    
    b2DestroyWorld(world);

    UnloadTexture(tilemap);
    UnloadNuklear(ctx);
    CloseWindow();
    
    return 0;
}
