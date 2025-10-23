// pair.hpp
#pragma once

#include <utility>
#include "core.hpp"
#include "io.hpp"

namespace nitron {

template <typename T1, typename T2>
class Pair {

public:
    inline Pair(ConstReference<T1> x_ = T1(), ConstReference<T2> y_ = T2()) :
        x(x_),
        y(y_)
    {}

    [[nodiscard]] inline ConstReference<T1> get_1() const { return x; }
    [[nodiscard]] inline ConstReference<T2> get_2() const { return y; }

    inline void set_1(ConstReference<T1> x_) { x = x_; }
    inline void set_2(ConstReference<T2> y_) { y = y_; }

    inline void unpack(Reference<T1> x_, Reference<T2> y_) const {
        x_ = x;
        y_ = y;
    }

    inline friend Reference<std::ostream> operator<<(Reference<std::ostream> xout, ConstReference<Pair> pair) requires is_printable<T1> and is_printable<T2> {
        return xout << pair.get_1() << ' ' << pair.get_2();
    }

private:
    T1 x;
    T2 y;
};

template<typename T1, typename T2>
inline Pair<T1,T2> nitronize(ConstReference<std::pair<T1,T2>> p) {
    return Pair<T1,T2>(p.first, p.second);
}

template<typename T1, typename T2>
inline std::pair<T1,T2> denitronize(ConstReference<Pair<T1,T2>> p) {
    return std::make_pair(p.get_1(), p.get_2());
}

} // namespace nitron