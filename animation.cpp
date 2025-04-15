#include "animation.h"

#include <raymath.h>

#include <cassert>

Animation::Animation(
  const std::string& file,
  int frames_horizontal,
  int frames_vertical,
  int frame_count,
  int fps
) {
  spritesheet_ = LoadTexture(file.c_str());

  assert(frames_horizontal > 0 && frames_vertical > 0 && "Invalid frame data");
  
  frames_horizontal_ = frames_horizontal;
  frames_vertical = frames_vertical;

  assert(fps > 0 && "Invalid frames per second");
  frames_per_second = 1.0f / fps;

  time_ = 0.0f;
  current_frame_ = 0;
  frame_count_ = frame_count;

  source_ = Rectangle {
    0.0f,
    0.0f,
    (float)spritesheet_.width / (float)frames_horizontal,
    (float)spritesheet_.height / (float)frames_vertical
  };

  frame_start_ = 0;
  frame_end_ = frame_count - 1;
}

Animation::Animation(Animation&& other){
  this->current_frame_ = other.current_frame_;
  this->frame_count_ = other.frame_count_;
  this->frames_horizontal_ = other.frames_horizontal_;
  this->frames_vertical_ = other.frames_vertical_;
  this->frames_per_second = other.frames_per_second;
  this->source_ = other.source_;
  this->time_ = other.time_;
  this->spritesheet_ = other.spritesheet_;

  other.free_ = false;
}

Animation::~Animation() {
  if (free_) {
    UnloadTexture(spritesheet_);
    free_ = false;
  }
}

void Animation::Update() {
  time_ += GetFrameTime();
  if (time_ >= frames_per_second) {
    time_ = 0.0f;
    ++current_frame_;
    if (current_frame_ > frame_end_) {
      current_frame_ = frame_start_;
    }
  }

  int x = current_frame_ % frames_horizontal_;
  int y = current_frame_ / frames_horizontal_;

  source_.x = x * source_.width;
  source_.y = y * source_.height;
}

void Animation::SetFrame(int frame) {
  frame = Clamp(frame, frame_start_, frame_end_);
  current_frame_ = frame;
  time_ = 0.0f;
}

int Animation::GetCurrentFrame() const {
  return current_frame_;
}

void Animation::Draw(Vector2 position, Vector2 origin, Vector2 size, float rotation) {
  DrawTexturePro(
    spritesheet_,
    source_,
    Rectangle { position.x, position.y, size.x, size.y },
    origin,
    rotation * RAD2DEG,
    WHITE
  );
}

int Animation::GetFrameCount() const {
  return frame_count_;
}

void Animation::SetFrameStart(int start) {
  frame_start_ = Clamp(start, 0, frame_count_ - 1);
  current_frame_ = frame_start_;
}

void Animation::SetFrameEnd(int end) {
  frame_end_ = Clamp(end, 0, frame_count_ - 1);
}

int Animation::GetFrameStart() const {
  return frame_start_;
}

int Animation::GetFrameEnd() const {
  return frame_end_;  
}
