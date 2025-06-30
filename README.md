# data::registry

[![linux](https://github.com/vladislavmarkov/data_registry/actions/workflows/linux.yml/badge.svg)](https://github.com/vladislavmarkov/data_registry/actions/workflows/linux.yml)
[![formatting](https://github.com/vladislavmarkov/data_registry/actions/workflows/formatting.yml/badge.svg)](https://github.com/vladislavmarkov/data_registry/actions/workflows/formatting.yml)
[![documentation](https://github.com/vladislavmarkov/data_registry/actions/workflows/docs.yml/badge.svg)](https://github.com/vladislavmarkov/data_registry/actions/workflows/docs.yml)

**data::registry** is a lightweight, header-only C++(>=14) library that lets users
obtain *read-only* or *read-write* access to data shared across translation
units, or to data returned by user-defined functions.

The core idea is to define data *entries* identified by a *tag*, a data type,
and, optionally, *read* and *write* accessors.

More detailed documentation is here: [Documentation](https://vladislavmarkov.github.io/data_registry)

## RAM usage

RAM usage is identical to using plain C++ static variables. Below is a
comparison of two binaries: the first represents a program with 2 000 static
variables, while the second represents a program with 2 000 entries added
through **data::registry**.
Both binaries were compiled with the *-O2* optimization flag.

|                   |   text |   data |   bss |   dec |   hex |
|-------------------|--------|--------|-------|-------|-------|
| Plain C++ statics |  61999 |    712 | 21608 | 84319 | 1495f |
|    data::registry |  61999 |    712 | 21608 | 84319 | 1495f |

## Quick Start

Step 1 - Declare entries

```cpp
#include <data/registry.h>

// static variable as an entry
reg_e(speed, unsigned int);
reg_e(user, user_t);

// static entries must be stored in a traslation unit
reg_store_e(speed); 

// you can provide init value or the entire list of constructor arguments
reg_store_e(user, "John", "Doe", "john.doe@jd.com");

// readonly entry accessible via reader
reg_e(temperature, float, read_temperature_sensor);

// read-write entry accessible via reader and writer
reg_e(logs, std::string, read_logs, write_logs); 
```

Step 2 - Access entries via `reg::get()` and `reg::set()` by tag

```cpp
// access function might have 0 to N number of parameters (contexts)
auto value = reg::get<tag>(ctx1, ..., ctxN);
reg::set<tag>(new_value, ctx1, ..., ctxN);
```

## Features

- **No exceptions**  
 The **data::registry** API never throws exceptions
 internally; any exceptions can originate only from user-provided functions.

- **Context data support**  
 When needed, callers may pass context objects to
 *reg::get()* or *reg::set()*, provided the corresponding user-provided readers
 or writers accept those contexts as parameters.

- **Compatibility**  
 Supported and tested on C++14 through C++23.

- **No generators**

- **No lookup tables**  
 Data is accessed at *O(1)*.
