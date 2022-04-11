/*  Report Candidates
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <algorithm>
#include "Kernels/XoroShiro1_Default.h"
#include "SeedScan.h"
#include "ReportCandidates.h"

namespace PokemonAutomation{


void search_candidates_thorough(
    SeedReporter& reporter,
    size_t rolls, uint32_t desired_pid,
    uint64_t start_seed, uint64_t iterations
){
    desired_pid &= 0xefffffff;
    for (uint64_t s = 0; s < iterations; s++){
        XoroShiroX1 rng0(start_seed);
        rng0.next();

        uint32_t pid0;
        for (size_t c = 0; c < rolls; c++){
#if 1
            pid0 = rng0.next_get_int32(0xffffffff, 0xffffffff) & 0xefffffff;
#else
            rng0.next();
            pid0 = rng0.get_int32() & 0xefffffff;
#endif
            if (pid0 == desired_pid){
                reporter.report(start_seed, c + 1);
            }
        }

        start_seed += 0x100000000;
    }
}



void search_candidates_normalPID(
    SeedReporter& reporter,
    size_t rolls, uint32_t desired_pid,
    uint64_t start_seed, uint64_t iterations
){
    if (iterations <= 65536){
        search_candidates_thorough(reporter, rolls, desired_pid, start_seed, iterations);
        return;
    }

    while (iterations > 0){
        uint64_t block = std::min(iterations, (uint64_t)65536);
        if (seed_scan_normalPID(rolls, desired_pid, start_seed, block)){
            search_candidates_thorough(reporter, rolls, desired_pid, start_seed, block);
        }
        iterations -= block;
        start_seed += block << 32;
    }
}
void search_candidates_maxPID(
    SeedReporter& reporter,
    size_t rolls, uint32_t desired_pid,
    uint64_t start_seed, uint64_t iterations
){
    if (iterations <= 65536){
        search_candidates_thorough(reporter, rolls, desired_pid, start_seed, iterations);
        return;
    }

    while (iterations > 0){
        uint64_t block = std::min(iterations, (uint64_t)65536);
        if (seed_scan_maxPID(rolls, desired_pid, start_seed, block)){
            search_candidates_thorough(reporter, rolls, desired_pid, start_seed, block);
        }
        iterations -= block;
        start_seed += block << 32;
    }
}




}
