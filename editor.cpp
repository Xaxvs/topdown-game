#include "editor.h"

#include <raymath.h>


LevelEditor::LevelEditor() {
  current_mode_ = EditMode::kNone;
  tilesheet_ = LoadNuklearImage("../assets/kenney_scribble-dungeons/Tilesheet/tilesheet.png");
  selected_ = nk_vec2i(0, 0);
  grid_width_ = 1;
  grid_height_ = 1;
  camera_ = Camera2D { Vector2 { 0.0, 0.0 }, Vector2 { 0.0, 0.0 }, 0.0, 1.0};
}

LevelEditor::~LevelEditor() {
  UnloadNuklearImage(tilesheet_);
}

void LevelEditor::Draw(struct nk_context* ctx) {
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
  struct nk_rect bounds = nk_rect(0, 0, 200, 200);
  if (nk_begin(ctx, "Grid Edit", bounds, NK_WINDOW_TITLE)) {
    nk_layout_row_dynamic(ctx, 15, 2);
    const char* grid_width_label = TextFormat("Grid Width: %d", grid_width_);
    nk_label(ctx, grid_width_label, NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &grid_width_, 100, 1);
    const char* grid_height_label = TextFormat("Grid Height: %d", grid_height_);
    nk_label(ctx, grid_height_label, NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &grid_height_, 100, 1);
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
