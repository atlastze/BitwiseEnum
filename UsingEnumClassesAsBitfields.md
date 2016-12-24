# Using Enum Classes as Bitfields
Using enum classes as bitfields/flags, supporting bitwise/bitmask operators.

form https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html

## Introduction
C++11 introduced a new feature in the form of **scoped enumerations** , also referred to as **enum classes**, since they are introduced with the double keyword `enum class` (though `enum struct` is also permissible, to identical effect). To a large extent, these are like standard enumerated types: you can declare a list of enumerators, which you may assign explicit values to, or which you may let the compiler assign values to. You can then assign these values to variables of that type. However, they have additional properties which make them ideal for use as bitfields. I recently answered a question on the accu-general mailing list about such a use, so I thought it might be worth writing a blog post about it.

## Key features of scoped enumerations
The key features provided by scoped enumerations are:

- The enumerators must always be prefixed with the type name when referred to outside the scope of the enumeration definition. e.g. for a scoped enumeration `colour` which has an enumerator green, this must be referred to as `colour::green` in the rest of the code. This avoids the problem of name clashes which can be common with plain enumerations.
- The underlying type of the enumeration can be specified, to allow forward declaration, and avoid surprising consequences of the compiler's choice. This is also allowed for plain `enum` in C++11. If no underlying type is specified for a scoped enumeration, the underlying type is fixed as `int`. The underlying type of a given enumeration can be found using the `std::underlying_type` template from the `<type_traits>` header.
- There is no implicit conversion to and from the underlying type, though such a conversion can be done explicitly with a cast.

This means that they are ideal for cases where there is a limited set of values, and there are several such cases in the C++ Standard itself: `std::errc`, `std::pointer_safety`, and `std::launch` for example. The lack of implicit conversions are particularly useful here, as it means that you cannot pass raw integers such as 3 to a function expecting a scoped enumeration: you have to pass a value of the enumeration, though this is of course true for unscoped enumerations as well. The lack of implicit conversions to integers does mean that you can overload a function taking a numeric type without having to worry about any potential ambiguity due to numeric conversion orderings.

## Bitmask types
Whereas the implicit conversions of plain enumerations mean that expressions such as `red | green` and `red & green` are valid if `red` and `green` are enumerators, the downside is that `red * green` or `red / green` are equally valid, if nonsensical. With scoped enumerations, none of these expressions are valid unless the relevant operators are defined, which means you can explicitly define what you want to permit.

`std::launch` is a scoped enumeration that is also a bitmask type. This means that expressions such as `std::launch::async | std::launch::deferred` and `std::launch::any & std::launch::async` are valid, but you cannot multiply or divide launch policies. The requirements on such a type are defined in section 17.5.2.1.3 [bitmask.types] of the C++ Standard, but they amount to providing definitions for the operators |, &, ^, ~, |=, &= and ^= with the expected semantics.

The implementation of these operators is trivial, so it is easy to create your own bitmask types, but having to actually define the operators for each bitmask type is undesirable.

## Bitmask operator templates
These operators can be templates, so you could define a template for each operator, e.g.

    template<typename E>
    E operator|(E lhs,E rhs){
        typedef typename std::underlying_type<E>::type underlying;
        return static_cast<E>(
            static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
    }

Then you could write `mask::x | mask::y` for some enumeration mask with enumerators `x` and `y`. The downside here is that it is too greedy: every type will match this template. Not only would you would be able to write `std::errc::bad_message | std::errc::broken_pipe`, which is clearly nonsensical, but you would also be able to write `"some string" | "some other string"`, though this would give a compile error on the use of `std::underlying_type`, since it is only defined for enumerations. There would also be potential clashes with other overloads of operator|, such as the one for `std::launch`.

What is needed is a constrained template, so only those types which you want to support the operator will match.

## SFINAE to the rescue
SFINAE is a term coined by David Vandevoorde and Nicolai Josuttis in their book *C++ Templates: The Complete Guide*. It stands for "Substitution Failure is Not an Error", and highlights a feature of expanding function templates during overload resolution: if substituting the template parameters into the function declaration fails to produce a valid declaration then the template is removed from the overload set without causing a compilation error.

This is a key feature used to constrain templates, both within the C++ Standard Library, and in many other libraries and application code. It is such a key feature that the C++ Standard Library even provides a library facility to assist with its use: `std::enable_if`.

We can therefore use it to constain our template to just those scoped enumerations that we want to act as bitmasks.

    template<typename E>
    struct enable_bitmask_operators{
        static constexpr bool enable=false;
    };

    template<typename E>
    typename std::enable_if<enable_bitmask_operators<E>::enable,E>::type
    operator|(E lhs,E rhs){
        typedef typename std::underlying_type<E>::type underlying;
        return static_cast<E>(
            static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
    }

If `enable_bitmask_operators<E>::enable` is `false` (which it is unless specialized) then `std::enable_if<enable_bitmask_operators<E>::enable,E>::type` will not exist, and so this operator| will be discarded without error. It will thus not compete with other overloads of operator|, and the compilation will fail if and only if there are no other matching overloads. `std::errc::bad_message | std::errc::broken_pipe` will thus fail to compile, whilst `std::launch::async | std::launch::deferred` will continue to work.

For those types that we do want to work as bitmasks, we can then just specialize `enable_bitmask_opoerators`:

    enum class my_bitmask{
        first=1,second=2,third=4
    }:
    template<>
    struct enable_bitmask_operators<my_bitmask>{
        static constexpr bool enable=true;
    };

Now, `std::enable_if<enable_bitmask_operators<E>::enable,E>::type` will exist when `E` is `my_bitmask`, so this operator| will be considered by overload resolution, and `my_bitmask::first | my_bitmask::second` will now compile.

## Final code
The final code is available as a header file along with a simple example demonstrating its use. It has been tested with g++ 4.7, 4.8 and 4.9 in C++11 mode, and with MSVC 2012 and 2013, and is released under the Boost Software License.
