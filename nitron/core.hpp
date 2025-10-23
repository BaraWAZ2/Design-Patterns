// core.hpp
#pragma once

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

} // namespace nitron