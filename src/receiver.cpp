#include "functions.hpp"
#include "numbers_shorthand.hpp"
#include "globals.hpp"
#include <mpi/mpi.h>
#include <vector>



/* save collision */
template<typename A_type, typename B_type, typename C_type>
void save_collision(set_type<A_type>& A,
                    set_type<B_type>& B,
                    set_type<C_type>& C,
                    A_type inp1,
                    B_type inp2,
                    C_type out){

}
// print output that we found something interesting
/* save the golden collisions */
template<typename A_type, typename B_type, typename C_type>
void probe_dict(u8* buff,
                set_type<A_type>& A,
                set_type<B_type>& B,
                set_type<C_type>& C){

}

template<typename A_type, typename B_type, typename C_type>
void receiver(mitm_functions<A_type, B_type, C_type> funcs,
            set_type<A_type>& A,
            set_type<B_type>& B,
            set_type<C_type>& C,
            size_t dict_size, /* how many triples in the dictionary */
            MPI_Comm local_comm,
            MPI_Comm inter_comm)
{
    /* initializes: dictionary, receive buffer, results buffer */
    u8 recv_buf[triple_length*n_msgs];
    dict d{dict_size};
    /* listen to senders */
    /* probe their outputs in the dictionary */
    /* if the golden collision is found exits */

}