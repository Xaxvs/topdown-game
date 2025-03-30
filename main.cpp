#include <raylib.h>
#include <raylib-nuklear.h>

#include <vector>

#include "tile.h"
#include "editor.h"
#include "Player.h"

#include <box2d/box2d.h>
#include <box2d/math_functions.h>

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

    Player player("Steve", 64, 64);

    editor.Load("dummylevel.txt", grid);
    int grid_width = editor.GetGridWidth();
    int grid_height = editor.GetGridHeight();

    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = b2Vec2_zero;

    b2WorldId world = b2CreateWorld(&world_def);
    std::vector<b2BodyId> bodies;
    std::vector<std::tuple<b2ShapeId, b2BodyId>> balls;

    constexpr float meter_to_pixel = 64.0;
    constexpr float pixel_to_meter = 1.0 / meter_to_pixel;
    
    for (int i = 0; i < grid.size(); ++i) {
        int x = i % grid_width;
        int y = i / grid_width;

        Tile tile = grid[i];
        if (!tile.is_wall_) continue;
        
        b2BodyDef body_def = b2DefaultBodyDef();
        body_def.position = b2Vec2 { (float)x + 0.5f, (float)y + 0.5f };
        b2BodyId wall = b2CreateBody(world, &body_def);

        float size = kGridSize * pixel_to_meter * 0.5f;
        
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
            b2Vec2 pos = b2Vec2 { player_pos.x * pixel_to_meter, player_pos.y * pixel_to_meter };
        
            b2BodyDef body_def = b2DefaultBodyDef();
            body_def.type = b2BodyType::b2_dynamicBody;
            body_def.position = b2Add(pos, b2MulSV(pixel_to_meter, dir));

            b2BodyId body = b2CreateBody(world, &body_def);

            b2ShapeDef shape_def = b2DefaultShapeDef();
            shape_def.density = 0.1;
            shape_def.restitution = 0.2;
            shape_def.friction = 0.0;

            radius = (float)GetRandomValue(5, 20);

            b2Circle circle { b2Vec2_zero, radius * pixel_to_meter };

            b2ShapeId shape = b2CreateCircleShape(body, &shape_def, &circle);

            b2Body_ApplyLinearImpulseToCenter(body, b2MulSV(pixel_to_meter * 2.0f, dir), true);
            
            balls.push_back(std::make_tuple(shape, body));
        }

        player.Update();
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode2D(editor.GetCamera());
        editor.DrawTileMap(grid, tilemap);        

        // for (int i = 0; i < grid.size(); ++i) {
        //     Tile tile = grid[i];
        //     int x = i % grid_width;
        //     int y = i / grid_width;

        //     Rectangle source {
        //         (float)tile.texture_col_ * kGridSize,
        //         (float)tile.texture_row_ * kGridSize,
        //         kGridSize,
        //         kGridSize
        //     };

        //     Rectangle dest {
        //        (float)x * kGridSize,
        //        (float)y * kGridSize,
        //        kGridSize,
        //        kGridSize
        //     };

        //     DrawTexturePro(tilemap, source, dest, Vector2 { 0.0, 0.0 }, 0.0f, WHITE);
        // }

        // for (b2BodyId body : bodies) {
        //     b2Vec2 pos = b2Body_GetPosition(body);
        //     DrawRectangle(pos.x * meter_to_pixel - 32, pos.y * meter_to_pixel - 32, kGridSize, kGridSize, RED);
        // }

        player.Draw(tilemap);
        
        for (auto& [shape, body]: balls) {
            b2Vec2 pos = b2Body_GetPosition(body);
            float r = b2Shape_GetCircle(shape).radius * meter_to_pixel;
            DrawCircle(pos.x * meter_to_pixel, pos.y * meter_to_pixel, r, ColorBrightness(RED, r / 25.0));
            DrawCircleLines(pos.x * meter_to_pixel, pos.y * meter_to_pixel, r, BLACK);
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
