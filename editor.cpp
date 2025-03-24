#include "editor.h"

#include <raymath.h>

#include <assert.h>


LevelEditor::LevelEditor() {
  current_mode_ = EditMode::kNone;
  tilesheet_ = LoadNuklearImage("../assets/kenney_scribble-dungeons/Tilesheet/tilesheet.png");
  selected_ = nk_vec2i(0, 0);
  grid_width_ = 1;
  grid_height_ = 1;
  
  camera_.offset = Vector2 { 0, 0 };
  camera_.rotation = 0.0;
  camera_.zoom = 1.0;
  camera_.target = Vector2 { 0, 0 };
}

LevelEditor::~LevelEditor() {
  UnloadNuklearImage(tilesheet_);
}

void LevelEditor::DrawUI(struct nk_context* ctx) {
  DrawText("E - Tile Select, R - Edit Grid Size, T - Draw Mode", 5, 0, 24, BLACK);
  DrawText("Middle Mouse Button - Pan, Scroll Wheel - Zoom", 5, 24, 24, BLACK);
  switch (current_mode_) {
    case EditMode::kTileSelect: {
      TileSelect(ctx);
      break;
    }
    case EditMode::kGridEdit: {
      GridEdit(ctx);
      break;    
    }
    case EditMode::kPaint: {
      DrawText("DRAW MODE ACTIVE (LMB - Floor, RMB - Wall)", 5, 48, 24, RED);
      break;    
    }
    case EditMode::kNone: {
      return;    
    }
  }
}

void LevelEditor::TileSelect(struct nk_context* ctx) {
  if (nk_begin(ctx, "Tile Select", nk_rect(0.0, 0.0, 896.0, 704.0), NK_WINDOW_TITLE)) {
      nk_layout_row_dynamic(ctx, 704, 1);
      nk_image(ctx, tilesheet_);

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
          if (col == selected_.x && row == selected_.y) {
            nk_stroke_rect(canvas, frame, 0.0, 5.0, nk_rgb(200, 255, 200));
          } else if (nk_input_is_mouse_hovering_rect(&ctx->input, frame) && mouse_over_window) {                   
            if (nk_input_is_mouse_click_in_rect(&ctx->input, NK_BUTTON_LEFT, frame)) {
              selected_ = nk_vec2(col, row);
            }
            nk_stroke_rect(canvas, frame, 0.0, 5.0, nk_rgb(200, 200, 255));
          } else {
            nk_stroke_rect(canvas, frame, 0.0, 1.5, nk_rgb(250, 120, 120));
          }
      
        }
      }

  }
  nk_end(ctx);
}

void LevelEditor::GridEdit(struct nk_context* ctx) {
  struct nk_rect bounds = nk_rect(0, 0, 400, 200);
  if (nk_begin(ctx, "Grid Edit", bounds, NK_WINDOW_TITLE)) {
    nk_layout_row_dynamic(ctx, 15, 2);
    const char* grid_width_label = TextFormat("Grid Width: %d", grid_width_);
    nk_label(ctx, grid_width_label, NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &grid_width_, 100, 1);
    const char* grid_height_label = TextFormat("Grid Height: %d", grid_height_);
    nk_label(ctx, grid_height_label, NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &grid_height_, 100, 1);
    
    nk_layout_row_static(ctx, 10, 350, 1);
    nk_label(ctx, "Press Enter to Clear and Resize", NK_TEXT_CENTERED);
  }
  nk_end(ctx);
}


void LevelEditor::SetMode(EditMode mode) {
  current_mode_ = mode;
}

int LevelEditor::GetGridWidth(void) const {
  return grid_width_;
}

int LevelEditor::GetGridHeight(void) const {
  return grid_height_; 
}

int LevelEditor::GetSelectedColumn(void) const {
  return selected_.x;
}

int LevelEditor::GetSelectedRow(void) const {
  return selected_.y;
}

EditMode LevelEditor::GetCurrentMode(void) const {
  return current_mode_;
}

void LevelEditor::UpdateCamera(void) {
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    camera_.offset.x += GetMouseDelta().x;
    camera_.offset.y += GetMouseDelta().y;
  }

  camera_.zoom += GetMouseWheelMove();
  camera_.zoom = Clamp(camera_.zoom, 0.5, 5.0);
}

Camera2D LevelEditor::GetCamera(void) const {
  return camera_;
}

void LevelEditor::Paint(std::vector<Tile>& grid) {
  Vector2 mouse_pos = GetMousePosition();  
  
  mouse_pos.x *= GetWindowScaleDPI().x;
  mouse_pos.y *= GetWindowScaleDPI().y;
  
  Vector2 world_pos = GetScreenToWorld2D(mouse_pos, camera_);

  int size = kGridSize;

  int col = (int)(world_pos.x / size);
  int row = (int)(world_pos.y / size);

  if (col < 0 || col >= grid_width_ || row < 0 || row >= grid_height_) {
    return;
  }
  
  int i = row * grid_width_ + col;
  if (i < 0 && i >= grid.size()) {
    return;
  }
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    grid.at(i) = Tile { (int)selected_.x, (int)selected_.y, false };
  } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
    grid.at(i) = Tile { (int)selected_.x, (int)selected_.y, true };
  }
}

void LevelEditor::DrawTileMap(const std::vector<Tile>& grid, Texture2D tilemap) const {
  DrawRectangle(0, 0, GetGridWidth() * kGridSize, GetGridHeight() * kGridSize, GRAY);

  for (int i = 0; i < grid.size(); ++i) {
    int column = i % GetGridWidth();
    int row = i / GetGridWidth();

    Tile tile = grid[i];
    int pos_x = column * kGridSize;
    int pos_y = row * kGridSize;

    Rectangle texture_rect;
    texture_rect.x = tile.texture_col_ * kGridSize;
    texture_rect.y = tile.texture_row_ * kGridSize;
    texture_rect.width = kGridSize;
    texture_rect.height = kGridSize;

    Rectangle dest_rect;
    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.width = kGridSize;
    dest_rect.height = kGridSize;
  
    Color tint = tile.is_wall_ ? GREEN : WHITE;
    DrawTexturePro(tilemap, texture_rect, dest_rect, Vector2{ 0.0, 0.0 }, 0.0, tint);
  }
}

void LevelEditor::UpdateEditor(std::vector<Tile>& grid) {
  if (GetCurrentMode() == EditMode::kNone) {
    UpdateCamera();
  }
  if (GetCurrentMode() == EditMode::kPaint) {
    UpdateCamera();
    Paint(grid);
  }

  if (IsKeyPressed(KEY_E)) {
    SetMode(EditMode::kTileSelect);
  } else if (IsKeyPressed(KEY_R)) {
    SetMode(EditMode::kGridEdit);
  } else if (IsKeyPressed(KEY_ESCAPE)) {
    SetMode(EditMode::kNone);
  } else if (IsKeyPressed(KEY_T)) {
    SetMode(EditMode::kPaint);
  }

  if (GetCurrentMode() == EditMode::kGridEdit) {
    if (IsKeyPressed(KEY_ENTER)) {
      grid.clear();
      grid.resize(GetGridWidth() * GetGridHeight());
    }
  }
}
