#include <mpi/mpi.h>
#include "functions.hpp"
#include "numbers_shorthand.hpp"
#define NMSGS 1000 // How many 
// global constants 

void sender(const size_t A_length,
	    const size_t B_length,
	    const size_t C_length,
	    MPI_Comm local_comm,
	    MPI_Comm inter_comm)
{

  /* Note: about *_length although these will be provided by functions.h */
  /* only mitm is responsible */

  /* ============================ INIT ====================================== */
  const size_t max_length = A_length > B_length ?
                     (A_length > C_length ? A_length : C_length) :
                     (B_length > C_length ? B_length : C_length) ;

  const size_t max_length_inp = A_length > B_length ? A_length : B_length;
  
  int myrank, nsenders;
  MPI_Comm_rank(local_comm, &myrank);
  MPI_Comm_size(local_comm, &nsenders);

  /* random message base */
  // where to get a_length, and b_length


  
  // inp_odd -f,g-> out := inp_even -f,g-> out := inp_even -> ...
  /* inp[] = A_part || B_part */
  u8 inp_odd[2*max_length]; // inp[A_length + B_length];
  u8 inp_even[2*max_length]; // out[2*C_length];

  /* We will be wasting bandwith if we keep space for inp_A and inp_B 
     instead we use  bit that says whether it's A or B + A or B + C ouptut */
  u8 snd_buf[(1 + max_length_inp + C_length)*NMSGS];

  /* Sample a random element */
  
  random_element_A(inp_odd, A_length);
  // Initially the second part of the input is same as the first part 
  

  
  /* ---------------------- GENERATE & SEND ------------------------------- */
  while (1) {
    /* odd -> even -> odd, while loop */
    /* Generate */
    /* f(out, out_lengt, inp, inp_length )*/
    f(inp_even, C_length, inp_odd, A_length);
    // 2nd part 
    /* g(out, out_lengt, inp, inp_length )*/
    g(&inp_even[C_length], C_length, &inp_odd[A_length], B_length);
    

    /* Decide: iterate by f or g?  */
    
    /* Add to send buffer */

    /* Send only if send buffer is full */

    /* repeat */
  }
  

}
