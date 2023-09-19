#ifndef SENDER_MITM
#define SENDER_MITM
#include "numbers_shorthand.h"
#include <mpi.h>
void sender(MPI_Comm local_comm, MPI_Comm inter_comm);
#endif
