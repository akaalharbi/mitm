#include <mpi/mpi.h>
#include <cstring>
#include "functions.hpp"
#include "numbers_shorthand.hpp"
#define NMSGS 1000 // How many
#define SND_OUTPUTS_TAG 0
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
  size_t is_dist;

  /* ============================ INIT ====================================== */
  const size_t max_length_inp = std::max(A.length, B.length);

  int myrank, nsenders, nreceivers;
  MPI_Comm_rank(local_comm, &myrank);
  MPI_Comm_size(local_comm, &nsenders);
  MPI_Comm_size(inter_comm, &nreceivers); /* todo check this is correct! */
  /* random message base */
  // where to get a_length, and b_length
  


  /* We will be wasting bandwidth if we keep space for inp_A and inp_B  */
  /* instead we use  bit that says whether it's A or B + A or B + C output */
  // mpi junk
  const int triple_length = (1 + max_length_inp + C.length);
  u8 snd_buf[nreceivers][triple_length*NMSGS]; /* does this gives */
  for (int i = 0; i<nreceivers; ++i) /* init buffer with zeros */
      memset(snd_buf[i], 0, triple_length*NMSGS);


  /* To know which buffer is ready to send */
  int receivers_countres[nreceivers];
  memset(receivers_countres, 0, nreceivers);
  int dest;
  size_t idx;



  /* Sample a random element from A */

  // inp_odd -f,g-> out := inp_even -f,g-> out := inp_even -> ...
  /* inp[] = A_part || B_part */
  //u8 inp_odd[2*max_length]; // inp[A_length + B_length];
  A_type inp_A {A.random()};
  B_type inp_B {B.random()}; /* dummy value for now */
  C_type out {C.random()}; /* dummy value for now */
  //random_element_A(inp_odd, A_length);
  // Initially the second part of the input is same as the first part 
  

  
  /* ---------------------- GENERATE & SEND ------------------------------- */
  while (true) {
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
          dest = C.extract_k_bits(out, C.length) % nreceivers;
          idx = receivers_countres[dest]; /* how many triples stored in dest buffer */


          if (f_or_g == 1){
              // snd_buf triple = f_or_g || inp || output
              snd_buf[idx*triple_length] = 1; /* i.e. we are sending f input */
              A.serialize(inp_A, &snd_buf[ 1 + idx*(triple_length) ]);

          } else{
              // snd_buf triple = f_or_g || inp || output
              snd_buf[idx*triple_length] = 0; /* i.e. we are sending g input */
              A.serialize(inp_B, &snd_buf[ 1 + idx*(triple_length) ]);
          }
          C.serialize(out, &snd_buf[ 1 + max_length_inp + idx*(triple_length) ]);
          ++receivers_countres[dest];

          if (receivers_countres[dest] == NMSGS){
              // send it
              MPI_Send(snd_buf,
                       NMSGS*triple_length,
                       MPI_CHAR,
                       SND_OUTPUTS_TAG,
                       inter_comm);

              receivers_countres[dest] = 0; /* todo careful when |A| < |B| or the reverse there will be some junk */
          }
      }

      /* repeat */
  }


}
