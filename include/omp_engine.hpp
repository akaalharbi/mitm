#ifndef MITM_ENGINE_OMP
#define MITM_ENGINE_OMP

#include <cmath>
#include <cassert>
#include <cstdio>
#include <optional>
#include <tuple>

#include <omp.h>

#include "common.hpp"
#include "engine_common.hpp"


/* multi-threaded collision-finding engine using OpenMP */

namespace mitm {

class OpenMPEngine {
public:

/* try to iterate for 1s. Return #it/s */
template<typename ConcreteProblem>
static double benchmark(const ConcreteProblem& Pb)
{
    u64 i = 42;
    u64 threshold = (1ull << (Pb.n - 1));
    double start = wtime();
    u64 total = 0;
    u64 k = 1000;
    #pragma omp parallel reduction(+:total)
    {
        u64 x = 0;
        double delta = 0.;
        while (delta < 1.) {
            for (u64 j = 0; j < k; j++) {
                u64 y = Pb.mixf(i, x);
                if (is_distinguished_point(y, threshold))
                    x = j;
                else
                    x = y;
            }
            total += k;
            delta = wtime() - start;
        }
    }
    return total / (wtime() - start);
}


/* The sequence of mixing function is deterministic given `prng` */
template<typename ConcreteProblem>
static std::tuple<u64,u64,u64> run(const ConcreteProblem& Pb, Parameters &params, PRNG &prng)
{
    Counters &ctr = Pb.ctr;

    printf("Benchmarking... ");
    fflush(stdout);
    double it_per_s = benchmark(Pb);
    char hitps[8];
    human_format(it_per_s, hitps);
    printf("%s iteration/s (using all cores)\n", hitps);


    Dict<std::pair<u64, u64>> dict(params.nbytes_memory);
    params.finalize(Pb.n, dict.n_slots);
    double log2_w = std::log2(dict.n_slots);

    printf("Starting collision search with seed=%016" PRIx64 ", difficulty=%.2f\n", prng.seed, params.difficulty);
    printf("Initialized a dict with %" PRId64 " slots = 2^%0.2f slots\n", dict.n_slots, log2_w);
    ctr.ready(Pb.n, dict.n_slots);
    printf("Expected iterations / collision = (2^%0.2f + 2^%.2f) \n", 
        Pb.n - params.difficulty - log2_w, 1 + params.difficulty);
    printf("Expected #iterations = (2^%0.2f + 2^%.2f) \n", 
        (Pb.n - 1) + (Pb.n - params.difficulty - log2_w), Pb.n + params.difficulty);
    printf("Generating %.1f*w = %" PRId64 " = 2^%0.2f distinguished point / version\n", 
        params.beta, params.points_per_version, std::log2(params.points_per_version));

    u64 i = 0;                 /* index of families of mixing functions */
    std::optional<std::tuple<u64,u64,u64>> solution;    /* (i, x0, x1)  */
    u64 n_dist_points = 0;     /* #DP found with this i */
    u64 root_seed = prng.rand();

    #pragma omp parallel    
    for (;;) {
        /* These simulations show that if 10w distinguished points are generated
         * for each version of the function, and theta = 2.25sqrt(w/n) then ...
         */

        for (;;) {
            u64 tmp;
            #pragma omp atomic read
            tmp = n_dist_points;
            if (tmp > params.points_per_version || solution)
                break;

            #pragma omp atomic
            n_dist_points += 1;

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

        #pragma omp barrier

        /* change the mixing function */
        #pragma omp single
        {
            n_dist_points = 0;
            i = prng.rand() & Pb.mask; 
            dict.flush();
            ctr.flush_dict();
            root_seed = prng.rand();
        }
    } // main loop
    return *solution;
}
};

}
#endif
