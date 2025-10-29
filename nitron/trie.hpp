// trie.hpp
#pragma once

#include <map>
#include <unordered_map>
#include "io.hpp"

namespace nitron {

template<typename T, bool order = std::totally_ordered<T>>
class Trie {

public:
    inline Trie(bool is_endpoint_ = false) :
        next(),
        is_endpoint(is_endpoint_)
    {}

    template<is_iterator Iterator>
    void insert(Iterator begin, Iterator end) 
    requires is_iterator_of_type<Iterator,T> {
        if (begin == end) {
            is_endpoint = true;
            return;
        }
        Reference<Trie> child = next[*begin];
        ++begin;
        child.insert(begin, end);
    }

    template<is_iterator Iterator>
    void erase(Iterator begin, Iterator end)
    requires is_iterator_of_type<Iterator,T> {
        if (begin == end) {
            is_endpoint = false;
            return;
        }
        auto next_it = next.find(*begin);
        if (next_it == next.end())
            return;
        Reference<Trie> child = next_it->second;
        ++begin;
        child.erase(begin, end);
        if (not child.is_endpoint and child.next.empty())
            next.erase(next_it);
    }

    [[nodiscard]] size_t count_endpoints() const {
        size_t ans = is_endpoint;
        auto end = next.end();
        for (
            auto begin = next.begin();
            begin != end;
            ++begin
        )
            ans += begin->second.count_endpoints();
        return ans;
    }

    template<is_iterator Iterator>
    [[nodiscard]] bool check(Iterator begin, Iterator end, bool exact_match = false) const
    requires is_iterator_of_type<Iterator,T> {
        if (begin == end)
            return not exact_match or is_endpoint;
        auto next_it = next.find(*begin);
        if (next_it == next.end())
            return false;
        ++begin;
        return next_it->second.check(begin, end, exact_match);
    }

    void print(Reference<std::ostream> xout, ConstReference<T> root, size_t depth = 0) const 
    requires is_printable<T> {
        xout
        << std::string(depth, '\t')
            << '[' << root << ']'
            << (is_endpoint ? "*" : " ")
            << std::endl;
        auto end = next.end();
        for (
            auto begin = next.begin();
            begin != end;
            ++begin
        )
            begin->second.print(xout, begin->first, depth + 1);
    }

    void print(Reference<std::ostream> xout, size_t depth = 0) const 
    requires is_printable<T> {
        xout
        << std::string(depth, '\t')
            << "[#]"
            << (is_endpoint ? "*" : " ")
            << std::endl;
        auto end = next.end();
        for (
            auto begin = next.begin();
            begin != end;
            ++begin
        )
            begin->second.print(xout, begin->first, depth + 1);
    }

    inline friend Reference<std::ostream> operator<<(Reference<std::ostream> xout, ConstReference<Trie> trie) {
        trie.print(xout);
        return xout;
    }

    template<is_container Container>
    [[nodiscard]] Container to_container_of_sequences() const 
    requires is_container_of_type<typename Container::value_type,T> {
        Container total;
        typename Container::value_type current;
        to_container_of_sequences(total, current);
        return total;
    }

protected:
    template<is_container Container>
    void to_container_of_sequences(Reference<Container> total, Reference<typename Container::value_type> current) const 
    requires is_container_of_type<typename Container::value_type,T> {
        if (is_endpoint)
            total.push_back(current);
        auto end = next.end();
        for (
            auto begin = next.begin();
            begin != end;
            ++begin
        ) {
            current.push_back(begin->first);
            begin->second.to_container_of_sequences(total, current);
            current.pop_back();
        }
    }

    std::conditional_t<order, std::map<T,Trie<T>>, std::unordered_map<T,Trie<T>>> next;
    bool is_endpoint;
};

typedef Trie<char> Dictionary;

template<is_iterator Iterator>
[[nodiscard]] inline Trie<typename Iterator::value_type::value_type> to_trie(Iterator begin, Iterator end)
requires is_container<typename Iterator::value_type> {
    Trie<typename Iterator::value_type::value_type> trie;
    while (begin != end) {
        trie.insert(begin->begin(), begin->end());
        ++begin;
    }
    return trie;
}

template<is_container Container>
inline void trie_sort(Reference<Container> container_of_sequences)
requires is_container<typename Container::value_type> {
    container_of_sequences = std::move(
        to_trie(
            container_of_sequences.begin(), container_of_sequences.end()
        ).template to_container_of_sequences<Container>()
    );
}

} // namespace nitron