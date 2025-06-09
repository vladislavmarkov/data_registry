#ifndef STORED_DATA_H
#define STORED_DATA_H

#include "data/registry.h"

#include <cstdint>

using stored_data = reg::registry<_e(speed, uint32_t), _e(temp, float)>;

#endif // STORED_DATA_H
