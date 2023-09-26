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
    for (size_t i = 0; i<n_msgs*triple_length; i += triple_length){
        collision = dict.search(&buff[i]);
        if (collision){ /* check is it the golden collision? */
            /* convert the input to A_type and B_type */
            A_type inp_A;
            B_type inp_B;

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
    std::array<u8, triple_length*n_msgs> rcv_buf{0}; /* rcv_buf.data() is a contiguous array*/

    Dictionary dict{dict_size};
    MPI_Status stat;
    int found_global = 0;
    /* listen to senders */
    MPI_Recv(rcv_buf.data(),
             triple_length*n_msgs,
             MPI_CHAR,
             MPI_ANY_SOURCE,
             SND_OUTPUTS_TAG,
             inter_comm,
             &stat);

    /* probe their outputs in the Dictionary */
    probe_insert_golden_dict(rcv_buf.data(), dict, A, B, C);
    /* if the golden collision is found exits */

}