#include <mpi/mpi.h>
#include "functions.hpp"
#include "numbers_shorthand.hpp"
#define NMSGS 1000 // How many 
// global constants 

template<typename A_type, typename B_type, typename C_type>
void sender(mitm_functions<A_type, B_type, C_type> funcs,
            set_type<A_type> A,
            set_type<B_type> B,
            set_type<C_type> C,
            int difficulty, /* n_bits that should = 0 to consider it as a distinguish point*/
            MPI_Comm local_comm,
            MPI_Comm inter_comm)
{

  /* Note: about *_length although these will be provided by functions.h */
  /* only mitm is responsible */

  /* here we should steal the resources. todo use move semantics */
  int f_or_g = 1; /* 1 if f, 0 if g */
  size_t is_dist = 0;

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
  A_type inp_A {A.random()};
  B_type inp_B {B.random()}; /* dummy value for now */
  C_type out {C.random()}; /* dummy value for now */

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
      if (f_or_g == 1){
          /* f(C_type& out, A_type& inp)*/
          funcs.f(inp_A, out);

          /* Decide: iterate by f or g?  */
          if (!C.extract_k_bits(inp_A, 1)) {
              f_or_g = 0; /* choose g */
              /* update the input for the next iteration */
              funcs.send_C_2_B(inp_B, out);
          } else {
              /* send_C_2_A(A_type& out_A, C_type& inp_C) */
              /* update the input for the next iteration */
              funcs.send_C_2_A(inp_A, out);
          }
          /* if we find a distinguish point, add it to the send buffer */

      } else { /* f_or_g == 0 */
          /* f(C_type& out, A_type& inp)*/
          funcs.g(inp_B, out);

          /* Decide: iterate by f or g?  */
          if (C.extract_k_bits(inp_B, 1)) {
              f_or_g = 1; /* choose f */ // todo
              /* update the input for the next iteration */
              funcs.send_C_2_A(inp_A, out);
          } else {
              /* send_C_2_A(A_type& out_A, C_type& inp_C) */
              /* update the input for the next iteration */
              funcs.send_C_2_B(inp_B, out);
          }


      }
      /* Do we have a distinguished point? */
      is_dist = C.extract_k_bits(out, difficulty);
      /* if yes, add it to the send buffer */
      if (is_dist == 0){
          /* Add to send buffer */

          /* Is send buffer full? */
          // if yes send
      }

      /* repeat */
  }


}
