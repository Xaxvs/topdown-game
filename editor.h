#ifndef EDITOR_H_
#define EDITOR_H_

#include <raylib-nuklear.h>
#include <raylib.h>

#include <vector>

#include "tile.h"

constexpr size_t kNumFramesHorizontal = 14;
constexpr size_t kNumFramesVertical = 11;
constexpr size_t kSpriteWidth = 63;
constexpr size_t kSpriteHeight = 65;
constexpr size_t kGridSize = 64;

enum class EditMode {
  kNone,
  kTileSelect,
  kGridEdit,
  kPaint,
  kSerialize
};

class LevelEditor {
public:
  LevelEditor();
  ~LevelEditor();

  void SetMode(EditMode mode);
  
  void DrawUI(struct nk_context* ctx);

  //Draw Tilemap that is being edited
  void DrawTileMap(const std::vector<Tile>& grid, Texture2D tilemap) const;

  //Updates editor state based on input
  void UpdateEditor(std::vector<Tile>& grid);

  int GetGridWidth(void) const;
  int GetGridHeight(void) const;
  int GetSelectedColumn(void) const;
  int GetSelectedRow(void) const;

  EditMode GetCurrentMode(void) const;

  void Paint(std::vector<Tile>& grid);
  
  void UpdateCamera(void);
  Camera2D GetCamera(void) const;
  
  void Load(const std::string& filename, std::vector<Tile>& grid);
private:
  void TileSelect(struct nk_context* ctx);
  void GridEdit(struct nk_context* ctx);
  void Serialize(struct nk_context* ctx);
private:
  enum class SerializationState {
    Inactive, //No need to save/load anything
    Save,
    Load
  };
  SerializationState serialize_state_;

  void Save(const std::vector<Tile>& grid);
private:
  struct nk_vec2 selected_;  
  struct nk_image tilesheet_;
  EditMode current_mode_;

  int grid_width_;
  int grid_height_;

  std::string filepath_;
  char filepath_buffer_[256];
  int filepath_len_;

  bool active_input_;


  Camera2D camera_;
};

#endif
