#ifndef FUNCTIONS_MITM
#define FUNCTIONS_MITM
#include "numbers_shorthand.h"
#include <stdio.h>
/// Conventions: output is 


/* find a collision between f and g */
/* do we need to provide the length of input  and output? */
void f(u8 *C_out, size_t C_length, u8 *A_inp, size_t A_inp_length);
void g(u8 *C_out, size_t C_length, u8 *B_inp, size_t B_inp_length);

/* a decision function for the golden collision. returns non-zero when golden */
/* e.g. when golden collision occurs, in double encrytpion */
int test_golden(u8 *A_inp, size_t A_inp_length, u8 *B_inp, size_t B_inp_length);

/* Normally, we have  A -> C, A -> C. Here we are looking for C -> A, C -> B */
/* Functions below should not be complicated. */
/* In case that output is shorter than the input, we need a way to extend it */
u8 *send_C_2_A(u8 *A_out, size_t A_length, u8 *C_inp, size_t C_length);
u8 *send_C_2_B(u8 *B_out, size_t B_length, u8 *C_inp, size_t C_length);


#endif 
