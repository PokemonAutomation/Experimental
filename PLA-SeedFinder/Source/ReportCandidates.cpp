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


void report_ec_pid_matches(
    SeedReporter& reporter,
    size_t rolls, uint32_t desired_pid,
    uint64_t start_seed, uint64_t iterations
){
    if (iterations <= 65536){
        for (uint64_t s = 0; s < iterations; s++){
            XoroShiroX1 rng0(start_seed);
            rng0.next();

            uint32_t pid0;
            for (size_t c = 0; c < rolls; c++){
                rng0.next();
                pid0 = rng0.get_int32();
                if (((pid0 ^ desired_pid) & 0xefffffff) == 0){
                    reporter.report(start_seed, c + 1);
                }
            }

            start_seed += 0x100000000;
        }
        return;
    }

    while (iterations > 0){
        uint64_t block = std::min(iterations, (uint64_t)65536);
        if (seed_scan(rolls, desired_pid, start_seed, block)){
            report_ec_pid_matches(reporter, rolls, desired_pid, start_seed, block);
        }
        iterations -= block;
        start_seed += block << 32;
    }
}




}
