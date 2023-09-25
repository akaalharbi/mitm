#include <mpi/mpi.h>
#include "functions.hpp"
#include "numbers_shorthand.hpp"
#define NMSGS 1000 // How many 
// global constants 

template<typename A_type, typename B_type, typename C_type>
void sender(set_type<A_type> A, /* A is considered the set of all elements of A_type */
            set_type<B_type> B, /* B is considered the set of all elements of B_type */
            set_type<C_type> C, /* C is considered the set of all elements of C_type */
            MPI_Comm local_comm,
            MPI_Comm inter_comm)
{

  /* Note: about *_length although these will be provided by functions.h */
  /* only mitm is responsible */

  /* ============================ INIT ====================================== */
  const size_t max_length = std::max(A.length, B.length, C.length);

  const size_t max_length_inp = std::max(A.length, B.length);

  int myrank, nsenders;
  MPI_Comm_rank(local_comm, &myrank);
  MPI_Comm_size(local_comm, &nsenders);

  /* random message base */
  // where to get a_length, and b_length


  
  // inp_odd -f,g-> out := inp_even -f,g-> out := inp_even -> ...
  /* inp[] = A_part || B_part */
  //u8 inp_odd[2*max_length]; // inp[A_length + B_length];
  std::pair<A_type, B_type> inp_odd {A.random(), B.random()};
  std::pair<A_type, B_type> inp_even{A.random(), B.random()}; /* dummy values */

  /* We will be wasting bandwith if we keep space for inp_A and inp_B 
     instead we use  bit that says whether it's A or B + A or B + C ouptut */
  u8 snd_buf[(1 + max_length_inp + C.length)*NMSGS];

  /* Sample a random element from A */

  
  //random_element_A(inp_odd, A_length);
  // Initially the second part of the input is same as the first part 
  

  
  /* ---------------------- GENERATE & SEND ------------------------------- */
  while (1) {
    /* odd -> even -> odd, while loop */
    /* Generate */
    /* f(C_type& out, A_type& inp)*/
    f(inp_even.first, inp_odd.first);
    // 2nd part
    /* g(C_type& out, B_type& inp) */
    g(inp_even.second, inp_odd.second);

    /* Decide: iterate by f or g?  */
    
    /* Add to send buffer */

    /* Send only if send buffer is full */

    /* repeat */
  }
  

}
