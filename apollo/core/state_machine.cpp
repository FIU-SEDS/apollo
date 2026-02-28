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
        current_state = transition.to;
        return;
      }
    }
  }
}