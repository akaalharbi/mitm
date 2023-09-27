#include "functions.hpp"
#include "numbers_shorthand.hpp"
#include "globals.hpp"
#include "dictionary.hpp"
#include <mpi/mpi.h>
#include <vector>


/* save collision */
template<typename A_type, typename B_type, typename C_type>
void save_collision(type_functions<A_type>& A,
                    type_functions<B_type>& B,
                    type_functions<C_type>& C,
                    A_type inp1,
                    B_type inp2,
                    C_type out){

}
// print output that we found something interesting
/* save the golden collisions */
template<typename A_type, typename B_type, typename C_type>
int probe_dict(u8* buff,
               Dictionary& dict,
               type_functions<A_type>& A,
               type_functions<B_type>& B,
               type_functions<C_type>& C)
{
    int collision = 0;
    /* Extracted triple from dict if we found a match */
    std::array<u8, triple_length>  result{0};
    int is_golden = 0;

    for (size_t i = 0; i<n_msgs*triple_length; i += triple_length){
        collision = dict.pop_insert(&buff[triple_length*i], result.data());
        if (collision){ /* check is it the golden collision? */
            /* convert the input to A_type and B_type */
            // todo continue here
            /* Check is it a golden collision */
            /* if golden save it in disk, exit */
            if (is_golden){
                return 1;
            }

        }
    }
}

template<typename A_type, typename B_type, typename C_type>
void receiver(mitm_functions<A_type, B_type, C_type> funcs,
              type_functions<A_type>& A,
              type_functions<B_type>& B,
              type_functions<C_type>& C,
              size_t dict_size, /* how many triples in the Dictionary */
            MPI_Comm local_comm,
              MPI_Comm inter_comm)
{
    /* initializes: Dictionary, receive buffer, results buffer */
    /* Allocate receiving buffer */
    std::vector<u8> rcv_buf(triple_length*n_msgs);
    int is_golden = 0;

    Dictionary dict{dict_size};
    MPI_Status stat;
    int found_global = 0;
    /* listen to senders */
    while (true) {
        MPI_Recv(rcv_buf.data(), // I think C++ standard guarantees that rcv_buf[i] is contiguous

                 triple_length * n_msgs,
                 MPI_CHAR,
                 MPI_ANY_SOURCE,
                 SND_OUTPUTS_TAG,
                 inter_comm,
                 &stat);

        /* probe their outputs in the Dictionary */
        is_golden = probe_dict(rcv_buf.data(), A, B, C);
        if (is_golden)
            return;
    }
// dict.pop_insert(rcv_buf.data(), result.data());
    /* if the golden collision is found exits */

}