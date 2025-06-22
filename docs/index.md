# data::registry

**data::registry** is a lightweight, header-only C++(>=14) library that lets users
obtain *read-only* or *read-write* access to data shared across translation
units, or to data returned by user-defined functions.

The core idea is to define data *entries* identified by a *tag*, a data type,
and, optionally, *read* and *write* accessors.

Step 1 - Declare entries

```cpp
// static variable as an entry
_e(tag, type);

// readonly entry accessible via reader
_e(tag, type, reader);

// read-write entry accessible via reader and writer
_e(tag, type, reader, writer); 
```

Step 2 - Access entries via `reg::get()` and `reg::set()` by tag

```cpp
// access function might have 0 to N number of parameters (contexts)
auto value = reg::get<type>(ctx1, ..., ctxN);
reg::set<type>(new_value, ctx1, ..., ctxN);
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

## Static entries

One way to use **data::registry** is to define entries as simple static
variables by *_e* macro with just two arguments - a tag and a type.

Example:

```cpp
// .h
_e(speed, unsigned short);
```

```cpp
// .cpp
_store_e(speed, unsigned short);
```

```cpp
// usage
void override_speed()
{
    reg::set<speed>(60U);

    std::cout << "current speed is " << reg::get<speed>() << '\n';
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
_e(speed, uint16_t, get_speed);
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

_e(temperature, float, get_temperature, set_temperature);
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

_e(temperature, float, get_room_temperature);
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
