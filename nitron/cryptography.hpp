// cryptography.hpp
#pragma once

#include <functional>
#include "core.hpp"

namespace nitron {

class CryptSystem {

public:
    typedef std::function<size_t(ConstReference<size_t>,ConstReference<size_t>)> function;

    CryptSystem(function encrypter_, function decrypter_) :
        encrypter(encrypter_),
        decrypter(decrypter_)
    {}

    [[nodiscard]] inline size_t encrypt(size_t plain_text, size_t key) const {
        return encrypter(plain_text, key);
    }

    [[nodiscard]] inline size_t decrypt(size_t cypher_text, size_t key) const {
        return decrypter(cypher_text, key);
    }

private:
    function encrypter;
    function decrypter;
};

} // namespace nitron