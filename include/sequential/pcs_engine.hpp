#ifndef MITM_ENGINE_SEQ
#define MITM_ENGINE_SEQ

#include <cmath>
#include <cstdio>

#include "common.hpp"
#include "engine_common.hpp"


/* multi-threaded collision-finding engine using OpenMP */

namespace mitm {

class SequentialEngine : Engine {
public:
using Counters = BaseCounters;

/* The sequence of mixing function is deterministic given `prng` */
template<typename ConcreteProblem>
static tuple<u64,u64,u64> run(ConcreteProblem& Pb, Parameters &params, PRNG &prng)
{
    Counters ctr;

    PcsDict dict(params.nbytes_memory);
    params.finalize(Pb.n, dict.n_slots);
    ctr.ready(Pb.n, dict.n_slots);
    double log2_w = std::log2(dict.n_slots);

    printf("Starting collision search with seed=%016" PRIx64 ", difficulty=%.2f\n", prng.seed, params.difficulty);
    printf("Initialized a dict with %" PRId64 " slots = 2^%0.2f slots\n", dict.n_slots, log2_w);
    printf("Expected iterations / collision = (2^%0.2f + 2^%.2f) \n", 
        Pb.n - params.difficulty - log2_w, 1 + params.difficulty);
    printf("Expected #iterations = (2^%0.2f + 2^%.2f) \n", 
        (Pb.n - 1) + (Pb.n - params.difficulty - log2_w), Pb.n + params.difficulty);
    printf("Generating %.1f*w = %" PRId64 " = 2^%0.2f distinguished point / version\n", 
        params.beta, params.points_per_version, std::log2(params.points_per_version));

    u64 i = 0;                 /* index of families of mixing functions */
    optional<tuple<u64,u64,u64>> solution;    /* (i, x0, x1)  */
    u64 n_dist_points = 0;     /* #DP found with this i */
    u64 root_seed = prng.rand();

    for (;;) {
        /* These simulations show that if 10w distinguished points are generated
         * for each version of the function, and theta = 2.25sqrt(w/n) then ...
         */

        for (;;) {
            if (n_dist_points > params.points_per_version || solution)
                break;

            /* start a new chain from a fresh "random" starting point */
            u64 start = (root_seed + n_dist_points) & Pb.mask;
            auto dp = generate_dist_point(Pb, i, params, start);
            if (not dp) {
                ctr.dp_failure();
                continue;       /* bad chain start ------ FIXME infinite loop if this happen */
            }

            auto [end, len] = *dp;
            ctr.found_distinguished_point(len);
            
            auto outcome = process_distinguished_point(Pb, ctr, dict, i, start, end, len);
            if (outcome)
                solution = outcome;
        }

        if (solution)
            break;

        /* change the mixing function */
        n_dist_points = 0;
        i = prng.rand() & Pb.mask; 
        dict.flush();
        ctr.flush_dict();
        root_seed = prng.rand();

    } // main loop
    return *solution;
}
};

}
#endif