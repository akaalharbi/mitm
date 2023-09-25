#ifndef DICT_MITM
#define DICT_MITM
#include "numbers_shorthand.hpp"
#include <stdio.h>


///-----------------------------------------------------///
///                  data structure                     ///
///-----------------------------------------------------///




typedef struct  { // todo think about replacement policy!
  /* store pair pair0=(key0, value0) in a seperate arrays for alignment. */
  /* The index of value0 in values is same as the index of key0 in keys. */
  u32* values; 
  u32* keys;
  size_t nelements; // total number of elements in the dictionary
  size_t nbuckets; 
  size_t nslots_per_bucket; // = nelements / nbuckets

  /* variables to check statistics */
  size_t nelements_asked_to_be_inserted;
  size_t nprobes_insert;
  size_t nprobes_lookup;

} dict;

dict *dict_creat(size_t nelements);
void dict_insert(u32 val, dict *d);
int dict_search(u32 val, dict *d);
void dict_free(dict *d);
size_t dict_memory(dict *d);



#endif
