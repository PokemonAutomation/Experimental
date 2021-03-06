/*  Seed Scan
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#ifndef PokemonAutomation_SeedFinder_PokemonLA_SeedScan_H
#define PokemonAutomation_SeedFinder_PokemonLA_SeedScan_H

#include <stdint.h>
#include <vector>
#include "Tools.h"
#include "ReportCandidates.h"

namespace PokemonAutomation{

void print_isa();

bool seed_scan_common(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations);
bool seed_scan_thorough(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations);

std::vector<Result> run_search_common(const PokemonStats& stats, size_t rolls, size_t threads);
std::vector<Result> run_search_thorough(const PokemonStats& stats, size_t rolls, size_t threads);


}
#endif
