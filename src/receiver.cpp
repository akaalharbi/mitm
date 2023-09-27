#include "functions.hpp"
#include "numbers_shorthand.hpp"
#include "globals.hpp"
#include "dictionary.hpp"
#include <mpi/mpi.h>
#include <vector>
#include <fstream>
#include <iostream>

/* save collision */
template<typename A_type, typename B_type, typename C_type>
int verify_collision(u8* inp_1_bytes,
                     u8* inp_2_bytes,
                     mitm_functions<A_type, B_type, C_type>& funcs,
                     type_functions<A_type>& A,
                     type_functions<B_type>& B,
                     type_functions<C_type>& C)
{
    /* static to make creation costs only once. */
    static A_type inp_A;
    static B_type inp_B;
    static C_type out_A_to_C, out_B_to_C;
    int is_golden = 0;

    /* first byte tells us the name of the function */
    if (inp_1_bytes[0] == inp_2_bytes[0])
        return 0; /* We are not looking for collision with the same function f,f or g,g*/
    /* now, we know inp_1_bytes is an input to a different function than inp_2_bytes */
    /* However, we wish to determine if inp_1_bytes is an input to f or g */
    if (inp_1_bytes[0] == 1 ){
        /* inp_1_bytes is an input of f */
        A.serial_to_type(inp_A, inp_1_bytes);
        /* inp_2_bytes is an input of g */
        B.serial_to_type(inp_B, inp_2_bytes);
    } else {
        /* inp_1_bytes is an input of g */
        B.serial_to_type(inp_B, inp_1_bytes);
        /* inp_2_bytes is an input of f */
        A.serial_to_type(inp_A, inp_2_bytes);
    }

    /* Final test to make! */
    return funcs.test_golden(inp_A, inp_B);




}
// print output that we found something interesting
/* save the golden collisions */
template<typename A_type, typename B_type, typename C_type>
int probe_dict(u8* buff,
               Dictionary& dict,
               const mitm_functions<A_type, B_type, C_type>& funcs,
               const type_functions<A_type>& A,
               const type_functions<B_type>& B,
               const type_functions<C_type>& C)
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
                // todo add more information for printing
                std::cout << "Found a golden collision!\n";

                /* just save the two triples in file data.bin */
                std::fstream fileOut("collisions.dat", ios::out | ios::binary);

                /* Write triple length, and always use 8 bytes to write it*/
                fileOut.write(reinterpret_cast<char*>(triple_length), sizeof(u64));
                fileOut.write(reinterpret_cast<char*>(&buff[triple_length*i]), triple_length);
                fileOut.write(reinterpret_cast<char*>(result.data()), triple_length);

                return 1;
            }

        }
    }
}

template<typename A_type, typename B_type, typename C_type>
void receiver(mitm_functions<A_type, B_type, C_type>& funcs,
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