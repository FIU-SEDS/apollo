#pragma once

#include <cstdint>

// Flight states
typedef enum : uint8_t
{
  INIT,
  IDLE,
  BOOST,
  BURNOUT,
  COAST,
  APOGEE,
  DESCENT_DROGUE,
  DESCENT_MAIN,
  LANDED
} SM_State;

/* TODO:
 *
 * A collection of states
 * Which state is currently active
 * A collection of transitions (each transition needs: where it comes FROM, where it goes TO, and a CONDITION that determines when it fires)
 */
