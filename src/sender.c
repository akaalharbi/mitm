#include <mpi.h>
#include <stddef.h>
#include "numbers_shorthand.h"
#define NMSGS 1000 // How many 
// global constants 

void sender(size_t A_length,
	    size_t B_length,
	    size_t C_length,
	    MPI_Comm local_comm,
	    MPI_Comm inter_comm)
{

  /* ============================ INIT ====================================== */
  size_t max_length = A_length > B_length ?
                     (A_length > C_length ? A_length : C_length) :
                     (B_length > C_length ? B_length : C_length) ;

  size_t max_length_inp = A_length > B_length ? A_length : B_length;
  
  int myrank, nsenders;
  MPI_Comm_rank(local_comm, &myrank);
  MPI_Comm_size(local_comm, &nsenders);

  /* random message base */
  // where to get a_length, and b_length

  u8 inp[2*max_length]; // inp[A_length + B_length];
  /* todo: optimization in the future: out and inp should have the same length  */
  /* This saves one copying in each iteration. */
  u8 out[2*max_length]; // out[2*C_length]; 
  /* We will be wasting bandwith if we keep space for inp_A and inp_B 
     instead we use  bit that says whether it's A or B + A or B + C ouptut */
  u8 snd_buf[(1 + max_length_inp + C_length)*NMSGS];



  /* ---------------------- GENERATE & SEND ------------------------------- */
  while (1) {
    /* Generate */

    /* Add to send buffer */

    /* Send only if send buffer is full */

    /* repeat */
  }
  

}
