//
// Global configurations for MPI communications
//

#ifndef MITM_GLOBALS_HPP
#define MITM_GLOBALS_HPP
//define NMSGS 1000 /* How many triples will be sent in one message */

/* Edit these based on the cipher */
constexpr int A_length = 64;
constexpr int B_length = 64;
constexpr int C_length = 64;

constexpr size_t max_length_inp = std::max(A_length, B_length);
constexpr int triple_length = (1 + max_length_inp + C_length);
#endif //MITM_GLOBALS_HPP
