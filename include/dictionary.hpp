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

/* use move semantics to init the Dictionary */
struct Dictionary { // todo think about replacement policy!
    /// A stupid Dictionary, however, from the outside it would be the same use regardless of internal optimizations.
    /* A shortcut that saves typing */

    /* Rappel: using KeyType = std::array<u8, 1 + max_length_inp + C_length>; */

    std::vector< u8 > vec;
    // variables to show statistics
    size_t n_slots; // total number of elements that a dictionary can have

    size_t n_elements_asked_to_be_inserted{0};
    size_t n_elements_popped{0};
    //size_t n_elements{0}; // #insertion -  #popped  =  how many different elements are actually in the dictionary.
    size_t idx{0};
    u8 zero[triple_length]{0}; /* zero triple*/

    Dictionary(size_t n_slots) : n_slots{n_slots} {
        vec.resize(n_slots*triple_length);
        /* Initialize the Dictionary to zeros */
        std::fill(vec.begin(), vec.end(), 0);
    }

    int pop_insert(u8* key, u8* out) {
        /* The main function we are going to use, it does insert and search at the same time! */
        idx = idx % n_slots;
        /* if we have a collision then return 1 */
        if (   std::memcmp(&vec[idx], zero, triple_length) != 0 ){
            // out = vec[idx]
            std::memcpy(out, &vec[idx], triple_length);
            ++n_elements_popped;
            ++n_elements_asked_to_be_inserted;
            return 1; /* We  found a collision */
        }

        // vec[idx] = key;
        std::memcpy(&vec[idx], key, triple_length);
        ++n_elements_asked_to_be_inserted;
        return 0; /* no collision was found */
    }
};

#endif
