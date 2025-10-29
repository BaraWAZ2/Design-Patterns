// core.hpp
#pragma once

#include <iostream>
#include <concepts>
#include <functional>

namespace nitron {

template <typename T>
using Pointer = T*;

template <typename T>
using ConstPointer = T* const;

template <typename T>
using PointerToConst = const T*;

template <typename T>
using ConstPointerToConst = const T* const;

template <typename T>
using Reference = T&;

template <typename T>
using ConstReference = const T&;

template<typename T>
concept is_printable = requires(T x, Reference<std::ostream> xout) {
    { xout << x } -> std::convertible_to<Reference<std::ostream>>;
};

template<typename T>
concept is_iterator = std::input_or_output_iterator<T>;

template<typename T>
concept is_container = requires(T container, typename T::iterator iterator) {
    typename T::iterator;
    typename T::reverse_iterator;
    typename T::value_type;
    requires is_iterator<typename T::iterator>;
    requires is_iterator<typename T::reverse_iterator>;
    { container.begin()  } -> std::same_as<typename T::iterator>;
    { container.end()    } -> std::same_as<typename T::iterator>;
    { container.rbegin() } -> std::same_as<typename T::reverse_iterator>;
    { container.rend()   } -> std::same_as<typename T::reverse_iterator>;
    { container.size()   } -> std::convertible_to<size_t>;
    { container.empty()  } -> std::same_as<bool>;
    { container = container            } -> std::convertible_to<Reference<T>>;
    { container = std::move(container) } -> std::convertible_to<Reference<T>>;
    T{iterator, iterator};
    T{container};
    T{std::move(container)};
};

template<typename C, typename E>
concept is_container_of_type = is_container<C> and std::same_as<typename C::value_type, E>;

template<typename C, typename I>
concept is_iterator_of_container = is_container<C> and is_iterator<I> and (
    std::same_as<typename C::iterator,I> or std::same_as<typename C::reverse_iterator,I> or
    std::same_as<typename C::const_iterator,I> or std::same_as<typename C::const_reverse_iterator,I>
);

template<typename I, typename T>
concept is_iterator_of_type = is_iterator<I> and std::same_as<typename I::value_type, T>;

template<typename T>
using binary_function = std::function<T(ConstReference<T>, ConstReference<T>)>;

} // namespace nitron