// segment_tree.hpp
#pragma once

#include "core.hpp"
#include "io.hpp"

namespace nitron {

template<typename T>
class SegmentTree1 {

public:
    using value_type = T;
    using merge_function = binary_function<T>;

    inline SegmentTree1(
        size_t data_size_,
        ConstReference<merge_function> merge_ = std::plus<value_type>(),
        ConstReference<value_type> identity_ = value_type()
    ) :
        data_size(data_size_),
        tree(data_size_ << 2ull, identity_),
        identity(identity_),
        merge([merge_, identity_](ConstReference<value_type> left, ConstReference<value_type> right){
            return left == identity_ ? right : right == identity_ ? left : std::move(merge_(left, right));
        })
    {}
    inline SegmentTree1(
        ConstReference<std::vector<value_type>> data,
        ConstReference<merge_function> merge_ = std::plus<value_type>(),
        ConstReference<value_type> identity_ = value_type()
    ) :
        data_size(data.size()),
        tree(data.size() << 2ull, identity_),
        identity(identity_),
        merge([merge_, identity_](ConstReference<value_type> left, ConstReference<value_type> right){
            return left == identity_ ? right : right == identity_ ? left : std::move(merge_(left, right));
        })
    {
        build(0, 0, data_size - 1, data);
    }
    
    inline value_type query(size_t data_left, size_t data_right) const {
        if (data_right < data_left) {
            if (data_right < 0 or data_left >= data_size)
                throw std::out_of_range(
                    "nitron::SegementTree1<T>::query() "
                    "with [T=" + std::string(typeid(value_type).name()) + "] "
                    "recieved an out of range index value."
                );
            return std::move(merge(
                query(0, data_right, 0, 0, data_size - 1),
                query(data_left, data_size - 1, 0, 0, data_size - 1)
            ));
        } else {
            if (data_left < 0 or data_right >= data_size)
                throw std::out_of_range(
                    "nitron::SegementTree1<T>::query() "
                    "with [T=" + std::string(typeid(value_type).name()) + "] "
                    "recieved an out of range index value."
                );
            return std::move(query(data_left, data_right, 0, 0, data_size - 1));
        }
    }
    
    inline void update(size_t query_index, ConstReference<value_type> new_value) {
        if (query_index < 0 or query_index >= data_size)
            throw std::out_of_range(
                    "nitron::SegementTree1<T>::update() "
                    "with [T=" + std::string(typeid(value_type).name()) + "] "
                    "recieved an out of range index value."
                );
        update(query_index, new_value, 0, 0, data_size - 1);
    }

    inline void print(Reference<std::ostream> xout, size_t indent = 0) const 
    requires is_printable<value_type> {
        print(xout, indent, 0, 0, data_size - 1);
    }

    inline friend Reference<std::ostream> operator<<(Reference<std::ostream> xout, ConstReference<SegmentTree1> segment_tree) {
        segment_tree.print(xout);
        return xout;
    };

private:
    void build(size_t tree_index, size_t data_left, size_t data_right, ConstReference<std::vector<value_type>> data) {
        if (data_left == data_right) {
            tree[tree_index] = data[data_left];
            return;
        }
        auto [data_middle, tree_left, tree_right] = get_traversal_info(tree_index, data_left, data_right);
        build(tree_left, data_left, data_middle, data);
        build(tree_right, data_middle + 1, data_right, data);
        tree[tree_index] = std::move(merge(tree[tree_left], tree[tree_right]));
    }
    
    void update(size_t query_index, ConstReference<value_type> new_value, size_t tree_index, size_t data_left, size_t data_right) {
        if (data_left == data_right and data_left == query_index) {
            tree[tree_index] = new_value;
            return;
        }
        if (query_index < data_left or data_right < query_index)
            return;
        auto [data_middle, tree_left, tree_right] = get_traversal_info(tree_index, data_left, data_right);
        update(query_index, new_value, tree_left, data_left, data_middle);
        update(query_index, new_value, tree_right, data_middle + 1, data_right);
        tree[tree_index] = std::move(merge(tree[tree_left], tree[tree_right]));
    }

    value_type query(size_t query_left, size_t query_right, size_t tree_index, size_t data_left, size_t data_right) const {
        if (query_left <= data_left and data_right <= query_right)
            return tree[tree_index];
        if (query_right < data_left or data_right < query_left)
        return identity;
        auto [data_middle, tree_left, tree_right] = get_traversal_info(tree_index, data_left, data_right);
        return std::move(merge(
            query(query_left, query_right, tree_left, data_left, data_middle),
            query(query_left, query_right, tree_right, data_middle + 1, data_right)
        ));
    }
    
    static std::array<size_t, 3ull> get_traversal_info(size_t tree_index, size_t data_left, size_t data_right) {
        return {
            data_left + ((data_right - data_left) >> 1ull),
            (tree_index << 1ull) + 1,
            (tree_index << 1ull) + 2
        };
    }

    inline void print(Reference<std::ostream> xout, size_t indent, size_t tree_index, size_t data_left, size_t data_right) const 
    requires is_printable<value_type> {
        xout << std::string(indent, '\t') << '[' << tree[tree_index] << ']';
        if (data_left == data_right) {
            xout << '*' << std::endl;
            return;
        }
        xout << std::endl;
        auto [data_middle, tree_left, tree_right] = get_traversal_info(tree_index, data_left, data_right);
        print(xout, indent + 1, tree_left, data_left, data_middle);
        print(xout, indent + 1, tree_right, data_middle + 1, data_right);
    }

    const size_t data_size;
    std::vector<value_type> tree;
    const value_type identity;
    const merge_function merge;
};

}