#pragma once

#include "state.h"
#include "sensor_data.h"
#include <vector>
#include <functional>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

class StateMachine
{

private:
  struct Transition
  {
    State *from;
    State *to;
    bool (*condition)(SensorData &); // function pointer
  };
  struct StateChange
  {
    std::string from_state;
    std::string to_state;
    std::string timestamp;
  };
  std::vector<State *> states;
  std::vector<Transition> transitions;
  std::vector<StateChange> history;
  State *current_state;

public:
  StateMachine();
  void addState(State *state);
  void setInitState(State *state);
  void defineTransition(State *from, State *to, bool (*condition)(SensorData &));
  void update(SensorData &sensors);
  void clearHistory();
  State *getCurrentState();
  std::vector<Transition> getAvailableTransitions() const;
  const std::vector<StateChange> &getHistory() const;
};
