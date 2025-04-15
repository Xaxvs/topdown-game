#ifndef PLAYER_STATES_H_
#define PLAYER_STATES_H_

#include "animation.h"
#include "state.h"

class PlayerStates;
class Player;

class IdleState : public State {
public:
  void OnEnter() override;
  void OnUpdate() override;
  void OnDraw(Vector2 position, Vector2 origin, Vector2 size, float rotation) override;
  void OnExit() override;
  
  PlayerStates* manager_;
  Player* player_;
private:
  float target_angle_;
};

class AttackState: public State {
public:  
  void OnEnter() override;
  void OnUpdate() override;
  void OnDraw(Vector2 position, Vector2 origin, Vector2 size, float rotation) override;

  PlayerStates* manager_;
  Player* player_;
};

class PlayerStates {
public:
  enum class IState {
    kIStateIdle,
    kIAttackState,
  };
  
  PlayerStates(Player* player);
  
  void Switch(IState state);
  
  void Update();
  void Draw(Vector2 position, Vector2 origin, Vector2 size, float rotation);

  IState GetCurrentState() const;

  Animation& GetAttackAnimation();
private:  
  Animation default_;
  
  IState current_state_type_;
  State* current_state_;
  
  IdleState idle_;
  AttackState attack_;
};

#endif
