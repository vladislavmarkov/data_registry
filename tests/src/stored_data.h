#ifndef STORED_DATA_H
#define STORED_DATA_H

#include "data/registry.h"

#include <cstdint>

extern "C" auto get_heartbeat() -> uint32_t;

enum class state_t : uint8_t
{
    initialization,
    operation_cycle,
    shutting_down
};

auto get_state() -> state_t;
void set_state(state_t);

_e(speed, uint16_t);
_e(temp, float);
_e(heartbeat, uint32_t, get_heartbeat);
_e(state, state_t, get_state, set_state);

#endif // STORED_DATA_H
