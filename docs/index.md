# data::registry

**data::registry** is a lightweight, header-only C++(>=14) library that lets users
obtain *read-only* or *read-write* access to data shared across translation
units, or to data returned by user-defined functions.

The core idea is to define data *entries* identified by a *tag*, a data type,
and, optionally, *read* and *write* accessors.

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

- **No heap allocation**

- **Context data support**  
 When needed, callers may pass context objects to
 *reg::get()* or *reg::set()*, provided the corresponding user-provided readers
 or writers accept those contexts as parameters.

- **Compatibility**  
 Supported and tested on C++14 through C++23.

- **No generators**

- **No lookup tables**  
 Data is accessed at *O(1)*.

## When to use

You might wonder why **data::registry** exists at all. If your project does not
need to manage multiple singleton-like objects that deliver either *static*
data or data available through *synchronous* calls, this API is probably not
the right fit. Conversely, when a project contains many such objects,
**data::registry** can provide a unified access point, while reducing the
amount of boilerplate code. If you plan to store only static values, consider
using a simpler mechanism — or reduce the number of global static variables
whenever possible — restricting data visibility to only those parts of the
program that actually use it is generally considered good practice.

## Static entries

One way to use **data::registry** is to define entries as simple static
variables by *reg_e()* macro with just two arguments - a tag and a type.

Example:

```cpp
// .h
reg_e(speed, unsigned short);
```

```cpp
// .cpp
/*
 * Here we store static variable associated with "speed"-tag in current
 * translation unit and (optionally) initialize with the value of 30.
 * If we don't specify the second argument, then the static value will
 * be default-constructed.
 */
reg_store_e(speed, 30);
```

```cpp
// usage
void override_speed()
{
    std::cout << "initial speed is " << reg::get<speed>() << '\n';

    reg::set<speed>(60U);

    std::cout << "speed is set to " << reg::get<speed>() << '\n';
}
```

!!! info "NOTE"

    For *static* entries, fundamental data types are returned by
    value, whereas more complex types are returned as *const references*. This
    policy avoids unnecessary copying when you call *reg::get<...>()*.

Another way to use **data::registry** is through **read** and **write**
accessors (*readers* & *writers*).

## *Readers* & *Writers*

If the data is accessible only through custom functions or callable objects —
for example, when it must be retrieved by a function that reads from hardware —
you can still expose it to **data::registry** by defining appropriate *reader*
and *writer*.

Example:

```cpp
// .h
reg_e(speed, uint16_t, get_speed);
```

```cpp
// usage
int foo()
{
    auto spd = reg::get<speed>();

    // ...
}
```

!!! info "NOTE"

    We don’t need to store the data in any translation unit with the
    *_store_e()* macro, because the *reader* (*get_speed()*) defines the data
    and is responsible for retrieving it from its source.

## Contexts

Sometimes, user needs to pass additional data while reading or writing the
value of an entry. It can be a single variable, or the whole set of them.
Let's say we need to read and write to the entry value in a thread-safe manner.
In such case we can pass a *synchronization primitive* as a context to both
reader and writer. The best way to do this is to pass `std::shared_mutex`
(C++17), or `boost::shared_mutex` (C++14) as a context to both accessor functions.

Example:

```cpp
// .h

// reads current temperature
float get_temperature(std::shared_mutex& mx);

// sets the temperature to desirable value
void set_temperature(float temp, std::shared_mutex& mx);
```

```cpp
// .cpp - possible implementation of accessors
float get_temperature(std::shared_mutex& mx)
{
    std::shared_lock lk(mx);
    return read_temperature();
}

void set_temperature(float temp, std::shared_mutex& mx)
{
    std::unique_lock lk(mx);
    set_pref_temperature(temp);
}

reg_e(temperature, float, get_temperature, set_temperature);
```

```cpp
// usage

static std::shared_mutex g_mx;

void manipulate_temperature()
{
    // ...

    std::cout << "temperature before " << reg::get<temperature>(g_mx) << '\n';

    reg::set<temperature>(20, g_mx);

    std::cout << "temperature after " << reg::get<temperature>(g_mx) << '\n';

    // ...
}
```

Keep in mind, that it's also possible to pass multiple contexts, in case if
user-defined readers and (or) writers have corresponding function parameters.

Example:

```cpp
// .h
enum class room_t : unsigned char {
    bathroom,
    bedroom,
    kitchen,
    livingroom
};

// let's say we have a reader function for a temperature for different
// appartments of a big appartment complex
float get_room_temperature(
    std::shared_mutex& mx,
    unsigned int appartment_no,
    room_t room);

reg_e(temperature, float, get_room_temperature);
```

```cpp
// .cpp
float get_room_temperature(
    std::shared_mutex& mx,
    unsigned int appartment_no,
    room_t room)
{
    std::shared_lock lk(mx);
    return read_sensor_data(appartment_no, room);
}
```

```cpp
// usage
static std::shared_mutex g_mx;

void check_all_kitchens()
{
    // iterating through all appartment numbers
    for (auto const apt_no : all_appartments)
    {
        if (reg::get<temperature>(g_mx, apt_no, room_t::kitchen) > 30.f)
        {
            notify_fire_dpt(apt_no);
        }
    }
}
```
