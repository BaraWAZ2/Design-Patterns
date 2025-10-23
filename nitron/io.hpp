// io.hpp
#pragma once

#include <iostream>
#include "core.hpp"
#include "concepts.hpp"

namespace nitron {

template<is_container Container>
inline Reference<std::ostream> operator<<(Reference<std::ostream> xout, ConstReference<Container> vector) requires (not std::same_as<Container, std::string>) {
    xout << "{ ";
    for (ConstReference<typename Container::value_type> element : vector)
        xout << element << ' ';
    return xout << '}';
}

template<is_iterator Iterator>
inline void print(Iterator begin, Iterator end, Reference<std::ostream> xout) {
    xout << "{ ";
    while (begin != end) {
        xout << *begin << ' ';
        ++begin;
    }
    xout << '}';
}

} // namespace nitron