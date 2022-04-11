/*  PLA SeedFinder
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <algorithm>
#include <mutex>
#include <iostream>
#include "Tools.h"
#include "SeedScan.h"
#include "ReportCandidates.h"
#include "DynamicParallelizer.h"
#include "PLA-SeedFinder.h"


using namespace PokemonAutomation;



int32_t pa_PLA_find_seeds(
    uint32_t pid, uint32_t ec, int8_t ivs[6], uint8_t max_rolls,
    uint64_t* seeds, uint8_t* rolls, uint32_t length
){
    return pa_PLA_find_seeds_threads(
        std::max<uint32_t>(std::thread::hardware_concurrency(), 1),
        pid, ec, ivs, max_rolls, seeds, rolls, length
    );
}

int32_t pa_PLA_find_seeds_threads(
    uint32_t threads,
    uint32_t pid, uint32_t ec, int8_t ivs[6], uint8_t max_rolls,
    uint64_t* seeds, uint8_t* rolls, uint32_t length
){
    PokemonStats stats;
    stats.pid = pid;
    stats.ec = ec;
    stats.ivs[0] = ivs[0];
    stats.ivs[1] = ivs[1];
    stats.ivs[2] = ivs[2];
    stats.ivs[3] = ivs[3];
    stats.ivs[4] = ivs[4];
    stats.ivs[5] = ivs[5];

    std::cout << std::endl;
    std::cout << "PLA Seed Finder v1.2" << std::endl;
    std::cout << std::endl;

    std::cout << "PID: " << tostr_hex_padded(stats.pid, 8) << std::endl;
    std::cout << "EC:  " << tostr_hex_padded(stats.ec, 8) << std::endl;
    std::cout << "IVs: " << stats.ivs[0] << " "
                         << stats.ivs[1] << " "
                         << stats.ivs[2] << " "
                         << stats.ivs[3] << " "
                         << stats.ivs[4] << " "
                         << stats.ivs[5] << std::endl;

    EcPidMatchReporter reporter(stats);

    std::cout << std::endl;
    print_isa();
    std::cout << "Threads: " << threads << std::endl;
    std::cout << std::endl;

    std::vector<Result> results;
    {
        std::cout << "Searching..." << std::endl;
        std::cout << std::endl;
        std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
        results = run_search_normalPID(stats, max_rolls, threads);
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
//        std::cout << std::endl;
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000. << " seconds        " << std::endl;
        std::cout << std::endl;
    }
    if (results.empty()){
        std::cout << "No seeds found. Retrying with -1 rerolls..." << std::endl;
        std::cout << std::endl;
        std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
        results = run_search_maxPID(stats, max_rolls, threads);
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
//        std::cout << std::endl;
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000. << " seconds        " << std::endl;
        std::cout << std::endl;
    }


    bool too_many = results.size() > length;
    if (!too_many){
        length = (uint32_t)results.size();
    }
    for (uint32_t c = 0; c < length; c++){
        seeds[c] = results[c].seed;
        rolls[c] = results[c].rolls;
    }
    return too_many ? -1 : length;
}
