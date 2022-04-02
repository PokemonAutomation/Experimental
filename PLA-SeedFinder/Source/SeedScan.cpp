/*  Seed Scan
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include "CpuFeatures.h"
#include "SeedScan.h"
#include "ReportCandidates.h"
#include "DynamicParallelizer.h"

#include <iostream>
using std::cout;
using std::endl;


std::string tostr_hex_padded(uint64_t x, size_t length = 16);


namespace PokemonAutomation{


bool seed_scan_Default(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations);
bool seed_scan_unroll4_SSE41(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations);
bool seed_scan_unroll8_AVX2(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations);
bool seed_scan_unroll16_AVX512(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations);


void print_isa(){
    if (CPU_CAPABILITY.OS_AVX512 && CPU_CAPABILITY.HW_AVX512_DQ){
        cout << "Instruction Set: AVX512" << endl;
        return;
    }
    if (CPU_CAPABILITY.OS_AVX && CPU_CAPABILITY.HW_AVX2){
        cout << "Instruction Set: AVX2" << endl;
        return;
    }
    if (CPU_CAPABILITY.HW_SSE41){
        cout << "Instruction Set: SSE4.1" << endl;
        return;
    }
    cout << "Instruction Set: Default" << endl;
}



bool seed_scan(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
#ifdef PA_AutoDispatch_17_Skylake
    if (CPU_CAPABILITY.OS_AVX512 && CPU_CAPABILITY.HW_AVX512_DQ){
        uint64_t block = iterations / 16 * 16;
        if (block > 0 && seed_scan_unroll16_AVX512(rolls, desired_pid, start_seed, block)){
            return true;
        }
        start_seed += block * 0x100000000;
        iterations -= block;
    }
#endif

#ifdef PA_AutoDispatch_13_Haswell
    if (CPU_CAPABILITY.OS_AVX && CPU_CAPABILITY.HW_AVX2){
        uint64_t block = iterations / 8 * 8;
        if (block > 0 && seed_scan_unroll8_AVX2(rolls, desired_pid, start_seed, block)){
            return true;
        }
        start_seed += block * 0x100000000;
        iterations -= block;
    }
#endif
    
#ifdef PA_AutoDispatch_08_Nehalem
    if (CPU_CAPABILITY.HW_SSE41){
        uint64_t block = iterations / 4 * 4;
        if (block > 0 && seed_scan_unroll4_SSE41(rolls, desired_pid, start_seed, block)){
            return true;
        }
        start_seed += block * 0x100000000;
        iterations -= block;
    }
#endif

    if (iterations > 0){
        return seed_scan_Default(rolls, desired_pid, start_seed, iterations);
    }
    return false;
}




std::vector<Result> run_search(const PokemonStats& stats, size_t rolls, size_t threads){
    EcPidMatchReporter reporter(stats);
    {
        DynamicParallelizer session(
            [&](uint64_t start, uint64_t length){
                report_ec_pid_matches(reporter, rolls, stats.pid, (uint32_t)(stats.ec - 0x82A2B175229D6A5B) + (start << 32), length);
            },
            65536, 0x100000000, threads
        );
    }
    return std::vector<Result>(reporter.seeds().begin(), reporter.seeds().end());
}




}
