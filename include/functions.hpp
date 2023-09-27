#ifndef FUNCTIONS_MITM
#define FUNCTIONS_MITM
#include "numbers_shorthand.hpp"
#include <cstddef>
#include <algorithm>
//#include "globals.hpp"

/* These three constants should be provided by user! */
constexpr int A_length = 64;
constexpr int B_length = 64;
constexpr int C_length = 64;

constexpr u64 max_length_inp = std::max(A_length, B_length);
/* triple = (is_f_or_g, input, output) */
/*           (1 byte, max_length_inp bytes, C_length bytes) */
constexpr u64 triple_length = (1 + max_length_inp + C_length);

/* to be deleted, we are using u8 arrays instead */
//using KeyType = std::array<u8, 1 + max_length_inp + C_length>;


template<typename X_type>
struct type_functions {
    /*  How many BYTES needed to code an arbitrary element of X */
    const static u64 length; // I feel it should be a private variable!

    auto random() -> X_type; /* get a random element from X */
    auto identity() -> X_type; /* Get an element that */
    auto extract_k_bits(X_type& elm, int n_bits) -> int; /* return k bits from the elm */
    /* Serialize an element of X to send for MPI */
    /* Should we pass a reference to avoid copying? */
    void serialize(u8 *out, X_type &inp); /* to send elm using MPI */

    /* Populate out with elements from inp, better than copying take the pointer inp inside out */
    void serial_to_type(X_type& out,  u8* inp);

    //auto is_distinguished(X elm, int diff) -> int;

};

template<typename A_type, typename B_type, typename C_type>
struct mitm_functions {


    /* INIT */
    /* */
    mitm_functions() {}

    /* Necessary functions provided by the user: */
    /* f: A -> C */
    void f(C_type& C_out, A_type& A_inp);
    /* g: B -> C */
    void g(C_type& C_out, B_type& B_inp);
    /* Did we find the golden collision? */
    int test_golden(A_type& A_inp, B_type& B_inp);
    /* Normally, we have  A -> C, B -> C. Here we are looking for C -> A, C -> B */
    /* Functions below should not be complicated, only a simple embedding. */
    /* In case that output is shorter than the input, we need a way to extend it */
    void send_C_to_A(A_type& A_out, C_type& C_inp); /* should we use move semantics to steal resources? todo */
    void send_C_to_B(B_type& B_out, C_type& C_inp); /* should we use move semantics to steal resources? todo */

};

#endif 
