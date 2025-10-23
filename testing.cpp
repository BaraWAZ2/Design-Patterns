#include "nitron/main.hpp"

void check_pair() {
    nitron::Pair<int,std::string> p (5, "Ahmad");
    {
        using namespace nitron;
        std::cout << p << std::endl;
    }
}

void check_trie() {
    nitron::Trie<char> trie;
    const std::vector<char> hello = {'h', 'e', 'l', 'l', 'o'};
    const std::vector<char> hella = {'h', 'e', 'l', 'l', 'a'};
    const std::vector<char> help  = {'h', 'e', 'l', 'p'};
    const std::vector<char> king  = {'k', 'i', 'n', 'g'};
    const std::string string = {'s', 't', 'r', 'i', 'n', 'g'};
    trie.insert(hello .begin(), hello .end());
    trie.insert(hella .begin(), hella .end());
    trie.insert(help  .begin(), help  .end());
    trie.insert(king  .begin(), king  .end());
    trie.insert(string.begin(), string.end());
    trie.erase (hella .begin(), hella .end());
    
    trie.print(std::cout);
    
    std::vector<std::vector<int>> sequences = {
        {1, 2, 3, 4, 5},
        {1, 2, 4, 8, 16},
        {1, 1, 2, 3, 5}
    };
    auto sequences_to_trie = nitron::to_trie(sequences.begin(), sequences.end());
    auto retrieved = sequences_to_trie.to_container_of_sequences<std::vector<std::vector<int>>>();
    std::sort(sequences.begin(), sequences.end());
    
    nitron::print(sequences.begin(), sequences.end(), std::cout);
    std::cout << std::endl;
    sequences_to_trie.print(std::cout);
    {
        using namespace nitron;
        nitron::print(retrieved.begin(), retrieved.end(), std::cout);
    }
    std::cout << std::endl;
    std::cout << "Does retrieved equal original? " << (sequences == retrieved) << std::endl;

    std::vector<std::vector<int>> vector_of_vectors = {
        { 1, 2, 3, 4, 5, 6, 7 },
        { 1, 3, 2, 4, 5, 6, 7 },
        { 1, 2, 3, 4, 6, 7, 5 }
    };
    nitron::trie_sort(vector_of_vectors);
    {
        using namespace nitron;
        std::cout << vector_of_vectors << std::endl;
    }
}

void check_cypher() {
    nitron::CryptSystem::function function = std::bit_xor<size_t>();
    nitron::CryptSystem master (function, function);
    
    {
        size_t plain     = 12345678910ull;
        size_t key       = 14392421053ull;
        size_t cypher    = master.encrypt(plain, key);
        size_t retrieved = master.decrypt(cypher, key);
        std::cout << plain << " -> " << cypher << " -> " << retrieved << std::endl;
    }
    
    nitron::CryptSystem::function caeser_enc = [](size_t p, size_t k) -> size_t {
        const size_t MASK = 0xFull;
        for (size_t i = 0; i < 16; ++i) {
            size_t shift = (i << 2ull);
            size_t value = ((p >> shift) & MASK);
            size_t key   = ((k >> shift) & MASK);
            size_t mask  = (MASK << shift);
            p &= ~mask;
            value = ((value + key) & MASK);
            p |= (value << shift);
        }
        return p;
    };

    nitron::CryptSystem::function caeser_dec = [](size_t c, size_t k) -> size_t {
        const size_t MASK = 0xFull;
        for (size_t i = 0; i < 16; ++i) {
            size_t shift = (i << 2ull);
            size_t value = ((c >> shift) & MASK);
            size_t key   = ((k >> shift) & MASK);
            size_t mask  = (MASK << shift);
            c &= ~mask;
            value = ((value + MASK + 1 - key) & MASK);
            c |= (value << shift);
        }
        return c;
    };

    nitron::CryptSystem server (caeser_enc, caeser_dec);

    {
        size_t plain     = 12345678910ull;
        size_t key       = 14392421053ull;
        size_t cypher    = server.encrypt(plain, key);
        size_t retrieved = server.decrypt(cypher, key);
        std::cout << plain << " -> " << cypher << " -> " << retrieved << std::endl;
    }
}

int main() {
    std::cout << std::boolalpha;
    
    check_pair();
    std::cout << std::endl;
    
    check_trie();
    std::cout << std::endl;
    
    check_cypher();
    std::cout << std::endl;
    
    std::cout << "Hello World" << std::endl;
    std::cout << "cplusplus version: " << __cplusplus << std::endl;
    return 0;
}