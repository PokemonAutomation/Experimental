/*  Report Candidates
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#ifndef PokemonAutomation_SeedFinder_PokemonLA_ReportCandidates_H
#define PokemonAutomation_SeedFinder_PokemonLA_ReportCandidates_H

#include <stdint.h>
#include <vector>
#include <mutex>
#include "Tools.h"

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


struct Result{
    uint64_t seed;
    uint8_t rolls;
    friend bool operator<(const Result& a, const Result& b){
        return a.seed < b.seed;
    }
};


class EcPidMatchReporter : public SeedReporter{
public:
    EcPidMatchReporter(const PokemonStats& stats)
        : m_stats(stats)
    {}

    const std::vector<Result>& seeds() const{
        return m_seeds;
    }

    virtual void report(uint64_t seed, size_t rolls) override{
        std::lock_guard<std::mutex> lg(m_lock);
//        cout << "seed = " << tostr_hex_padded(seed, 16) << ", rolls = " << rolls << endl;
//        print(seed, rolls, 0);

//        PokemonStats stats3 = generate(seed, rolls, 3);

        for (uint8_t c = 0; c < 6; c++){
            test_ivs(seed, rolls, c);
        }
    }

private:
    bool test_ivs(uint64_t seed, size_t rolls, uint8_t max_ivs){
        PokemonStats stats = generate(seed, rolls, max_ivs);
        if (matches(m_stats, stats)){
            m_seeds.emplace_back(Result{seed, (uint8_t)rolls});
            print(seed, rolls, max_ivs);
            return true;
        }
        return false;
    }

private:
    const PokemonStats& m_stats;
    std::mutex m_lock;
    std::vector<Result> m_seeds;
};





}
#endif
