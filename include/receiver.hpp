#ifndef RECEIVER_MITM
#define RECEIVER_MITM
#include "numbers_shorthand.hpp"
#include <mpi/mpi.h>
void receiver(MPI_Comm local_comm, MPI_Comm inter_comm);
#endif
