#include <limits>
#include <cassert>
#include <algorithm>
#include "nitron/main.hpp"

void check_segment() {
    using namespace nitron;
    std::cout << "=== SegmentTree1 Comprehensive Test ===" << std::endl;

    // --- 1. Test integer segment tree (sum) ---
    SegmentTree1<int> sum_tree({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, std::plus<int>(), 0);
    assert(sum_tree.query(0, 9) == 55);  // full sum
    assert(sum_tree.query(3, 3) == 4);   // single element
    assert(sum_tree.query(1, 4) == 14);  // range sum 2+3+4+5

    // Circular query (e.g., [8,2] wraps around)
    assert(sum_tree.query(8, 2) == (9 + 10 + 1 + 2 + 3)); 

    // Update test
    sum_tree.update(5, -10); // change index 5 (6 -> -10)
    assert(sum_tree.query(0, 9) == 55 - 6 - 10); // total now 39
    assert(sum_tree.query(4, 6) == (5 + (-10) + 7)); // =2
    sum_tree.print(std::cout);

    std::cout << "[OK] Integer sum tests passed.\n" << std::endl;

    // --- 2. Test integer segment tree (min) ---
    SegmentTree1<int> min_tree(
        {9, 3, 8, 6, 7, 4, 10},
        [](ConstReference<int> a, ConstReference<int> b){ return std::min(a, b); },
        std::numeric_limits<int>::max()
    );
    assert(min_tree.query(0, 6) == 3);
    assert(min_tree.query(2, 4) == 6);
    min_tree.update(1, 11);
    assert(min_tree.query(0, 6) == 4);
    std::cout << "[OK] Integer min tests passed.\n" << std::endl;

    // --- 3. Test integer segment tree (max) ---
    SegmentTree1<int> max_tree(
        {2, 9, 5, 1, 3, 8},
        [](ConstReference<int> a, ConstReference<int> b){ return std::max(a, b); },
        std::numeric_limits<int>::lowest()
    );
    assert(max_tree.query(0, 5) == 9);
    max_tree.update(1, 0);
    assert(max_tree.query(0, 5) == 8);
    std::cout << "[OK] Integer max tests passed.\n" << std::endl;

    // --- 4. Test string concatenation ---
    SegmentTree1<std::string> str_tree(
        {"ahmad", "ayman", "yaser", "hamzah"},
        [](ConstReference<std::string> s1, ConstReference<std::string> s2){ 
            return s1 + " " + s2;
        },
        std::string(" ")
    );

    auto s = str_tree.query(0, 3);
    assert(s.find("ahmad") != std::string::npos);
    assert(s.find("hamzah") != std::string::npos);
    str_tree.update(1, "ali");
    auto s2 = str_tree.query(0, 3);
    assert(s2.find("ali") != std::string::npos);
    str_tree.print(std::cout);
    std::cout << "[OK] String concatenation tests passed.\n" << std::endl;

    // --- 5. Exception safety ---
    bool caught = false;
    try {
        sum_tree.query(100, 200); // out of range
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
    caught = false;
    try {
        sum_tree.update(999, 42); // out of range
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
    std::cout << "[OK] Exception tests passed.\n" << std::endl;

    std::cout << "=== All SegmentTree1 Tests Passed Successfully ===" << std::endl;
}

int main() {
    std::cout << std::boolalpha;

    check_segment();
    std::cout << std::endl;

    std::cout << "Hello World" << std::endl;
    std::cout << "cplusplus version: " << __cplusplus << std::endl;
    return 0;
}