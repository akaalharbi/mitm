#ifndef DICT_MITM
#define DICT_MITM
#include "numbers_shorthand.hpp"
#include <cstddef>
#include <vector>

///-----------------------------------------------------///
///                  data structure                     ///
///-----------------------------------------------------///

// todo this incomplete.

/* use move semantics to init the dict */
template<typename C_type>
struct dict { // todo think about replacement policy!
    /* store pair pair0=(key0, value0) in a seperate arrays for alignment. */
    /* The index of value0 in values is same as the index of key0 in keys. */
    std::vector<std::pair<u64, C_type> > vec;
    // old code
    size_t n_elements; // total number of elements in the dictionary
    size_t n_buckets{};
    size_t n_slots_per_bucket{}; // = nelements / nbuckets

    /* variables to check statistics */
    size_t n_elements_asked_to_be_inserted{};
    size_t n_probes_insert{};
    size_t n_probes_lookup{};
    // end of the old code


    dict(size_t n_elements) : n_elements{n_elements} {
        vec.resize(n_elements);
    }

    auto search(std::pair<u64, u64>) -> int {
        return 0;
    }

    void insert(u64 val) {
        n_buckets = nBuckets;
    }
};

///* todo move all these functions inside */
//dict dict_create(size_t nelements);
//void dict_insert(u32 val, dict *d);
//int dict_search(u32 val, dict *d);
//void dict_free(dict *d);
//size_t dict_memory(dict *d);



#endif
