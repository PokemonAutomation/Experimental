/*  Seed Scan
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#pragma once
#ifndef PokemonAutomation_SeedFinder_PokemonLA_SeedScan_H
#define PokemonAutomation_SeedFinder_PokemonLA_SeedScan_H

#include <stdint.h>

namespace PokemonAutomation{

void print_isa();

bool seed_scan(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations);



}
#endif
