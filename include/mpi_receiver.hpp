#ifndef MITM_MPI_RECEIVER
#define MITM_MPI_RECEIVER

#include <vector>
#include <mpi.h>

#include "mpi_common.hpp"
#include "engine_common.hpp"

namespace mitm {

/* Manage reception buffers for a collection of sender processes, with double-buffering */
class RecvBuffers : public BaseRecvBuffers {
public:
	RecvBuffers(const MpiParameters &params) : BaseRecvBuffers(params.inter_comm, TAG_POINTS, 3 * params.buffer_capacity) {}
};

template<typename ConcreteProblem>
void receiver(const ConcreteProblem& Pb, const MpiParameters &params)
{
    MpiCounters &ctr = Pb.ctr; 
    Dict<std::pair<u64, u64>> dict(params.nbytes_memory / params.recv_per_node);

    double last_ping = wtime();

	for (;;) {
		/* get data from controller */
		u64 msg[3];   // i, root_seed, stop?
		MPI_Bcast(msg, 3, MPI_UINT64_T, 0, params.world_comm);
		if (msg[2] != 0)
			return;      // controller tells us to stop	
		u64 i = msg[0];

		// receive and process data from senders
		RecvBuffers recvbuf(params);
		for (;;) {
			if (recvbuf.complete())
				break;                      // all senders are done
			auto ready = recvbuf.wait(ctr);
			// process incoming buffers of distinguished points
			for (auto it = ready.begin(); it != ready.end(); it++) {
				auto & buffer = *it;
				for (size_t k = 0; k < buffer->size(); k += 3) {
					u64 start = buffer->at(k);
					u64 end = buffer->at(k + 1);
					u64 len = buffer->at(k + 2);
					assert((start & Pb.mask) == start);
					assert((end & Pb.mask) == end);
					auto solution = process_distinguished_point(Pb, ctr, dict, i, start, end, len);
					if (solution) {          // call home !
						auto [i, x0, x1] = *solution;
						u64 golden[3] = {i, x0, x1};
						MPI_Send(golden, 3, MPI_UINT64_T, 0, TAG_SOLUTION, params.world_comm);
					}
				}
			}
			
			/* call home? */
			if (wtime() - last_ping >= 1) {
            	last_ping = wtime();
            	u64 stats[3] = {ctr.n_points, ctr.n_collisions, (u64) (ctr.recv_wait * 1e6)};
            	// todo: send stats:  [#bytes received?]
            	MPI_Send(stats, 3, MPI_UINT64_T, 0, TAG_RECEIVER_CALLHOME, params.world_comm);
            	ctr.reset();
            }
		}
		dict.flush();
	}
}

}
#endif
