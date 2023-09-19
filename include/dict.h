#ifndef DICT_MITM
#define DICT_MITM
#include "numbers_shorthand.h"
#include <cstddef>
#include <stdio.h>


///-----------------------------------------------------///
///                  data structure                     ///
///-----------------------------------------------------///

typedef struct  { // todo alignement
  u32* values;
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
