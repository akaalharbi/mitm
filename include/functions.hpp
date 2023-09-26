#ifndef FUNCTIONS_MITM
#define FUNCTIONS_MITM
#include "numbers_shorthand.hpp"
//#include "globals.hpp"

template<typename X_type>
struct set_type {
    /*  How many BYTES needed to code an arbitrary element of X */
    const static size_t length; // I feel it should be a private variable!



    auto random() -> X_type; /* get a random element from X */
    auto identity() -> X_type; /* Get an element that */
    auto extract_k_bits(X_type& elm, int n_bits) -> int; /* return k bits from the elm */
    /* Serialize an element of X to send for MPI */
    /* Should we pass a reference to avoid copying? */
    void serialize(X_type& elm, u8* out); /* to send elm using MPI */

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
    void send_C_2_A(A_type& A_out, C_type& C_inp); /* should we use move semantics to steal resources? todo */
    void send_C_2_B(B_type& B_out, C_type& C_inp); /* should we use move semantics to steal resources? todo */

};

#endif 
