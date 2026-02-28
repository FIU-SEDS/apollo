#pragma once

#include "state.h"
#include "sensor_data.h"
#include <vector>
#include <functional>

class StateMachine
{

private:
  struct Transition
  {
    State *from;
    State *to;
    bool (*condition)(SensorData &); // function pointer
  };
  std::vector<State *> states;
  std::vector<Transition> transitions;
  State *current_state;

public:
  StateMachine();
  void addState(State *state);
  void setInitState(State *state);
  void defineTransition(State *from, State *to, bool (*condition)(SensorData &));
  void update(SensorData &sensors);
  State *getCurrentState();
};
