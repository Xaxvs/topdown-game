#include "editor.h"

#include <raymath.h>

#include <fstream>
#include <sstream>

#include <assert.h>


LevelEditor::LevelEditor(PhysicsWorld* world) {
  current_mode_ = EditMode::kNone;
  tilesheet_ = LoadNuklearImage("../assets/kenney_scribble-dungeons/Tilesheet/tilesheet.png");
  selected_ = nk_vec2i(0, 0);
  grid_width_ = 1;
  grid_height_ = 1;
  
  camera_.offset = Vector2 { 0, 0 };
  camera_.rotation = 0.0;
  camera_.zoom = 1.0;
  camera_.target = Vector2 { 0, 0 };
  filepath_.resize(256);

  active_input_ = false;

  assert(world != nullptr);
  physics_ = world;
  filepath_len_ = 0;

  box_select_anchor_ = {};
}

LevelEditor::~LevelEditor() {
  UnloadNuklearImage(tilesheet_);
}

void LevelEditor::DrawUI(struct nk_context* ctx) {
  DrawText("E - Tile Select, R - Edit Grid Size, T - Draw Mode, Y - Save/Load", 5, 0, 18, BLACK);
  DrawText("Middle Mouse Button - Pan, Scroll Wheel - Zoom", 5, 18, 18, BLACK);
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
      DrawText("Ctrl -> Box Draw", 5, 72, 24, RED);
      break;    
    }
    case EditMode::kSerialize: {
      Serialize(ctx);
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
    nk_slider_int(ctx, 1, &grid_width_, 500, 1);
    const char* grid_height_label = TextFormat("Grid Height: %d", grid_height_);
    nk_label(ctx, grid_height_label, NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &grid_height_, 500, 1);
    
    nk_layout_row_static(ctx, 10, 350, 1);
    nk_label(ctx, "Press Enter to Clear and Resize", NK_TEXT_CENTERED);
  }
  nk_end(ctx);
}



void LevelEditor::LevelEditor::SetMode(EditMode mode)  {
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

std::vector<Vector2> GetBoxRegion(Vector2 start, Vector2 end, int map_width, int map_height) {
  std::vector<Vector2> region;
  
  int x_dif = end.x - start.x;
  int y_dif = end.y - start.y;

  int start_x = start.x;
  int start_y = start.y;

  int end_x = start.x + x_dif;
  int end_y = start.y + y_dif;

  if (x_dif < 0) {
    end_x = start_x;
    start_x += x_dif;
  }
  if (y_dif < 0) {
    end_y = start_y;
    start_y += y_dif;
  }

  for (int x = start_x; x <= end_x; ++x) {
    for (int y = start_y; y <= end_y; ++y) {  
      if (x < 0 || x >= map_width || y < 0 || y >= map_height) {
        continue;
      }
      region.push_back(Vector2 { (float)x, (float)y });
    }
  }

  return region;
}

Vector2 GetMouseGridPos(Camera2D camera) {
  Vector2 mouse;
  mouse.x = GetMouseX() * GetWindowScaleDPI().x;
  mouse.y = GetMouseY() * GetWindowScaleDPI().y;
  mouse = GetScreenToWorld2D(mouse, camera);

  mouse.x = floorf(mouse.x / kGridSize);
  mouse.y = floorf(mouse.y / kGridSize);

  return mouse;
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
  if (i < 0 || i >= grid.size()) {
    return;
  }

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (is_box_select_) {
      if (!box_select_anchor_.has_value()) {
        box_select_anchor_ = Vector2 { (float)col, (float)row };
        return;
      } else {
        std::vector<Vector2> box = GetBoxRegion(
                                     box_select_anchor_.value(),
                                     GetMouseGridPos(camera_),
                                     grid_width_,
                                     grid_height_
                                   );
        for (Vector2 tile : box) {
          int i = tile.y * grid_width_ + tile.x;
          grid.at(i) = Tile { (int)selected_.x, (int)selected_.y, false };
        }
      }
      is_box_select_ = false;
      physics_->UpdateWalls(grid, grid_width_);
    }
  }
  
  if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
    if (box_select_anchor_.has_value()) {    
      std::vector<Vector2> box = GetBoxRegion(
                                   box_select_anchor_.value(),
                                   GetMouseGridPos(camera_),
                                   grid_width_,
                                   grid_height_
                                 );
      for (Vector2 tile : box) {
        int i = tile.y * grid_width_ + tile.x;
        grid.at(i) = Tile { (int)selected_.x, (int)selected_.y, true };
      }
      is_box_select_ = false;
      physics_->UpdateWalls(grid, grid_width_);
    }
  }
  
  if (is_box_select_) return;
    
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {        
    if (grid.at(i).is_wall_) {
      physics_->DestroyWall(i);
    }
    grid.at(i) = Tile { (int)selected_.x, (int)selected_.y, false };
  } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
    physics_->AddWall(i, GetGridWidth());
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

  if (current_mode_ == EditMode::kPaint) {
    int row = GetSelectedRow();
    int column = GetSelectedColumn();

    Rectangle source;
    source.x = row * kGridSize;
    source.y = column * kGridSize;
    source.width = kGridSize;
    source.height = kGridSize;

    if (box_select_anchor_.has_value()) {
      Vector2 pos = box_select_anchor_.value();

      Vector2 mouse = GetMouseGridPos(camera_);

      std::vector<Vector2> region = GetBoxRegion(pos, mouse, grid_width_, grid_height_);

      Color color = ColorAlpha(SKYBLUE, 0.9);
      for (Vector2 tile : region) {
        DrawTexturePro(
          tilemap,
          source,
          Rectangle {
            tile.x * kGridSize,
            tile.y * kGridSize,
            kGridSize,
            kGridSize
          },
          Vector2Zero(),
          0.0f,
          color
        );
      }
    }
      
    Rectangle dest;
    dest.x = GetMouseX() * GetWindowScaleDPI().x;
    dest.y = GetMouseY() * GetWindowScaleDPI().y;
    dest.width = kGridSize;
    dest.height = kGridSize;

    Vector2 world = GetScreenToWorld2D(Vector2 { dest.x, dest.y }, GetCamera());
    dest.x = world.x - kGridSize * 0.5;
    dest.y = world.y - kGridSize * 0.5;

    DrawTexturePro(tilemap, source, dest, Vector2{0.0, 0.0}, 0.0, ColorAlpha(WHITE, 0.5));
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

  if (active_input_) return;

  if (IsKeyPressed(KEY_E)) {
    SetMode(EditMode::kTileSelect);
  } else if (IsKeyPressed(KEY_R)) {
    SetMode(EditMode::kGridEdit);
  } else if (IsKeyPressed(KEY_ESCAPE)) {
    SetMode(EditMode::kNone);
  } else if (IsKeyPressed(KEY_T)) {    
    SetMode(EditMode::kPaint);
  } else if (IsKeyPressed(KEY_Y)) {
    SetMode(EditMode::kSerialize);
  }
  
  if (!is_box_select_) {
    box_select_anchor_ = {};
  }

  if (GetCurrentMode() == EditMode::kGridEdit) {
    if (IsKeyPressed(KEY_ENTER)) {
      grid.clear();
      grid.resize(GetGridWidth() * GetGridHeight());
      physics_->UpdateWalls(grid, GetGridWidth());
    }
  }

  is_box_select_ = (current_mode_ == EditMode::kPaint) && IsKeyDown(KEY_LEFT_CONTROL);
  
  switch (serialize_state_) {
    case SerializationState::Save: {
      Save(grid);
      serialize_state_ = SerializationState::Inactive;
      break;
    }
    case SerializationState::Load: {
      Load(filepath_, grid);
      serialize_state_ = SerializationState::Inactive;
      break;    
    }
    case SerializationState::Inactive: {
      break;    
    }
  }
}

void LevelEditor::Save(const std::vector<Tile>& grid) {
  std::ofstream file(filepath_);
  if (!file.is_open()) {
    TraceLog(LOG_ERROR, "Unable to open [%s]", filepath_.c_str());
    return;
  }

  file << TextFormat("%d %d\n", grid_width_, grid_height_);
  for (const Tile& tile : grid) {
    file << TextFormat("%d %d %d\n", tile.texture_col_, tile.texture_row_, tile.is_wall_);
  }

  file.close();
  TraceLog(LOG_INFO, "Saved to file successfully [%s].", filepath_.c_str());
}

void LevelEditor::Load(const std::string& filename, std::vector<Tile>& grid) {
  std::fstream file(filename);
  if (!file.is_open()) {
    TraceLog(LOG_ERROR, "Unable to open [%s]", filename.c_str());
    return;
  }

  std::string line;
  
  std::getline(file, line);
  std::istringstream input(line);

  input >> grid_width_ >> grid_height_;
  
  grid.clear();
  while (std::getline(file, line)) {
    input = std::istringstream(line);
    
    int column = 0;
    int row = 0;
    bool is_wall = false;
    
    input >> column >> row >> is_wall;
    grid.emplace_back(Tile { row, column, is_wall });
  }

  TraceLog(LOG_INFO, "Loaded level successfully [%s]", filepath_.c_str());
  
  physics_->UpdateWalls(grid, grid_width_);
}

void LevelEditor::Serialize(struct nk_context* ctx) {
  struct nk_rect bounds = nk_rect(0, 0, 800.0, 200.0);
  if (nk_begin(ctx, "Save/Load", bounds, NK_WINDOW_TITLE)) {
    nk_layout_row_dynamic(ctx, 40.0, 2);
    nk_label(ctx, TextFormat("Filepath (Char: %d/256)", filepath_len_), NK_TEXT_ALIGN_LEFT);
    ctx->style.edit.cursor_size = 1;
    
    if (nk_input_is_mouse_hovering_rect(&ctx->input, nk_widget_bounds(ctx))) {
      if (nk_tooltip_begin(ctx, 400.0)) {
        nk_layout_row_dynamic(ctx, 50.0, 1);
        nk_label_wrap(ctx, "Weird graphics bug where text field doesn't render last character.");
        nk_label_wrap(ctx, "The text below will show true output, don't worry :)");
        nk_tooltip_end(ctx);
      }
    }
    nk_flags text_edit_state = nk_edit_string(
                                ctx,
                                NK_EDIT_SIMPLE |
                                NK_EDIT_SELECTABLE |
                                NK_EDIT_CLIPBOARD |
                                NK_EDIT_GOTO_END_ON_ACTIVATE,
                                filepath_buffer_,
                                &filepath_len_,
                                sizeof(filepath_buffer_) - 1,
                                nk_filter_default
                              );    
    
    active_input_ = text_edit_state & NK_EDIT_ACTIVE;

    filepath_ = std::string(filepath_buffer_, filepath_len_);

    nk_layout_row_static(ctx, 20.0, 18 * filepath_len_, 1);
    nk_label_colored(ctx, filepath_.c_str(), NK_TEXT_ALIGN_LEFT, nk_rgb(250, 250, 250));

    nk_layout_row_dynamic(ctx, 40.0, 2);
        
    if (nk_button_label(ctx, "Save")) {
      serialize_state_ = SerializationState::Save;      
    }
    if (nk_button_label(ctx, "Load")) {
      serialize_state_ = SerializationState::Load;      
    }
  }
  nk_end(ctx);
}
