#ifndef EDITOR_H_
#define EDITOR_H_

#include <raylib-nuklear.h>
#include <raylib.h>

constexpr size_t kNumFramesHorizontal = 14;
constexpr size_t kNumFramesVertical = 11;
constexpr float kSpriteWidth = 63;
constexpr float kSpriteHeight = 65;

enum class EditMode {
  kNone,
  kTileSelect,
  kGridEdit,
  kPaint
};

class LevelEditor {
public:
  LevelEditor();
  ~LevelEditor();

  void SetMode(EditMode mode);
  void Draw(struct nk_context* ctx);

  int GetGridWidth(void) const;
  int GetGridHeight(void) const;
  int GetSelectedColumn(void) const;
  int GetSelectedRow(void) const;

  EditMode GetCurrentMode(void) const;
  
  void UpdateCamera(void);
  Camera2D GetCamera(void) const;
private:
  void TileSelect(struct nk_context* ctx);
  void GridEdit(struct nk_context* ctx);
private:
  struct nk_vec2 selected_;  
  struct nk_image tilesheet_;
  EditMode current_mode_;

  int grid_width_;
  int grid_height_;

  Camera2D camera_;
};

#endif
