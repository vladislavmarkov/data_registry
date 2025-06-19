# data::registry

**data::registry** is a lightweight, header-only C++(>=14) library that lets users
obtain *read-only* or *read-write* access to data shared across translation
units, or to data returned by user-defined functions.

The core idea is to define data *entries* identified by a *tag*, a data type,
and, optionally, *read* and *write* accessors.

```cpp
_e(tag, type);
```

or

```cpp
_e(tag, type, reader, writer);
```

In the near future, *contexts* will be supported for both *readers* and
*writers*.

```cpp
_e(tag, type, reader, reader_ctx, writer, writer_ctx);
```

## Statics

One way to use **data::registry** is to define entries as simple static variables.

Example:

```cpp
// .h
_e(speed, unsigned short);

// .cpp
_store_e(speed, unsigned short);

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

If the data is accessible only through custom functions — for example, when it
must be retrieved by a function that reads from hardware — you can still expose
it to **data::registry** by defining appropriate *reader* and *writer*.

Example:

```cpp
// .h
_e(speed, uint16_t, get_speed);

// usage
int foo()
{
    reg::get<speed>();
}
```

!!! info "NOTE"

    We don’t need to store the data in any translation unit with the
    *_store_e()* macro, because the *reader* (*get_speed()*) defines the data
    and is responsible for retrieving it from its source.
