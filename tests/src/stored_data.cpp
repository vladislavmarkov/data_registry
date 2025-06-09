#include "stored_data.h"

#include "data/registry.h"

_store_e(speed, uint16_t);
_store_e(temp, float);

using namespace std;

static auto _g_start = 0U;

extern "C" auto get_heartbeat() -> uint32_t { return ++_g_start; }

static auto _g_state = state_t::initialization;

auto get_state() -> state_t { return _g_state; }

void set_state(state_t st) { _g_state = st; }
