#include "state_machine.h"

StateMachine::StateMachine()
{
  current_state = NULL;
}

void StateMachine::addState(State *state)
{
  states.push_back(state);
}

void StateMachine::setInitState(State *state)
{
  current_state = state;
}

State *StateMachine::getCurrentState()
{
  return current_state;
}

void StateMachine::defineTransition(State *from, State *to, bool (*condition)(SensorData &))
{
  Transition t = {from, to, condition};
  transitions.push_back(t);
}

void StateMachine::update(SensorData &sensors)
{
  for (auto &transition : transitions)
  {
    if (transition.from == current_state)
    {
      if (transition.condition(sensors))
      {
        // Record the state change
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        auto local_time = std::localtime(&time_t_now);

        std::stringstream ss;
        ss << std::setfill('0')
           << std::setw(2) << local_time->tm_hour << ":"
           << std::setw(2) << local_time->tm_min << ":"
           << std::setw(2) << local_time->tm_sec;

        StateChange change;
        change.from_state = current_state->name;
        change.to_state = transition.to->name;
        change.timestamp = ss.str();
        history.push_back(change);

        // Now change the state
        current_state = transition.to;
        return;
      }
    }
  }
}

std::vector<StateMachine::Transition> StateMachine::getAvailableTransitions() const
{
  std::vector<Transition> available;

  for (const auto &transition : transitions)
  {
    if (transition.from == current_state)
    {
      available.push_back(transition);
    }
  }

  return available;
}

const std::vector<StateMachine::StateChange> &StateMachine::getHistory() const
{
  return history;
}

void StateMachine::clearHistory()
{
  history.clear();
}