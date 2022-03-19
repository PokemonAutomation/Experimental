/*  Report Candidates
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#pragma once
#ifndef PokemonAutomation_SeedFinder_PokemonLA_ReportCandidates_H
#define PokemonAutomation_SeedFinder_PokemonLA_ReportCandidates_H

#include <stdint.h>

namespace PokemonAutomation{


    
class SeedReporter{
public:
    virtual void report(uint64_t seed, size_t rolls) = 0;
};


void report_ec_pid_matches(
    SeedReporter& reporter,
    size_t rolls, uint32_t desired_pid,
    uint64_t start_seed, uint64_t iterations
);





}
#endif
