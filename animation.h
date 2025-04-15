#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <raylib.h>
#include <string>

class Animation {
public:
  Animation() = default;
  Animation(const std::string& file, int frames_horizontal, int frames_vertical, int frame_count, int fps = 24);
  Animation(Animation&& other);
  
  ~Animation();

  void Update();
  
  void SetFrame(int frame);
  int GetCurrentFrame() const;
  int GetFrameCount() const;

  void SetFrameStart(int start);
  void SetFrameEnd(int end);

  int GetFrameStart() const;
  int GetFrameEnd() const;
  
  void Draw(Vector2 position, Vector2 origin, Vector2 size, float rotation);
private:
  Texture2D spritesheet_;
  Rectangle source_;

  int frame_start_;
  int frame_end_;

  int current_frame_;
  int frames_horizontal_;
  int frames_vertical_;
  int frame_count_;

  float time_;
  float frames_per_second;

  bool free_ = true;
};


#endif
