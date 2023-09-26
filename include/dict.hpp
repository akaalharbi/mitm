#ifndef DICT_MITM
#define DICT_MITM
#include "numbers_shorthand.hpp"
#include <cstddef>
#include <cstring>
#include <vector>
#include <array>
#include <algorithm>
#include "functions.hpp"

///-----------------------------------------------------///
///                  data structure                     ///
///-----------------------------------------------------///

// todo this incomplete.

/* use move semantics to init the dict */
template<typename C_type>
struct dict { // todo think about replacement policy!
    /// A stupid dictionary, however, from the outside it would be the same use regardless of internal optimizations.
    /* A shortcut that saves typing */
    using InpType = std::array<u8, 1 + max_length_inp + C_length> ;
    std::vector< InpType > vec;
    // variables to show statistics
    size_t n_elements{0}; // total number of elements in the dictionary
    size_t n_elements_asked_to_be_inserted{0};
    size_t idx{0};
    InpType zero{0};

    dict(size_t n_elements) : n_elements{n_elements} {
        vec.resize(n_elements);
        /* Initialize the dictionary to zeros */
        std::fill(vec.begin(), vec.end(), zero);
    }

    auto search(InpType& key) -> int {
        std::memcpy(&idx, key.data(), sizeof(u64));
        idx = idx % (1 + max_length_inp + C_length);

        if (vec[idx] == key)
            return 1;
        return 0;
    }

    void insert(InpType key) { /* yes, we are copying the key here */
        std::memcpy(&idx, key.data(), sizeof(u64));
        idx = idx % (1 + max_length_inp + C_length);

        if (vec[idx] == zero)/* we are not overwriting an existing slot */
            ++n_elements; /* we have an extra element */
        ++n_elements_asked_to_be_inserted;
        vec[idx] = key; /* another copying occurs here, ouch! */
    }
};

#endif
