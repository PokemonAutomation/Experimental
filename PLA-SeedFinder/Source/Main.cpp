
#include <functional>
#include <chrono>
#include <algorithm>
#include <vector>
#include <set>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "SeedScan.h"
#include "Kernels/XoroShiro1_Default.h"
#include "ReportCandidates.h"
#include "DynamicParallelizer.h"
#include "Tools.h"

#include <iostream>
using std::cout;
using std::endl;

using namespace PokemonAutomation;





class EcPidMatchReporter : public SeedReporter{
public:
    EcPidMatchReporter(const PokemonStats& stats)
        : m_stats(stats)
    {}

    virtual void report(uint64_t seed, size_t rolls) override{
        std::lock_guard<std::mutex> lg(m_lock);
//        cout << "seed = " << tostr_hex_padded(seed, 16) << ", rolls = " << rolls << endl;
//        print(seed, rolls, 0);

        PokemonStats stats3 = generate(seed, rolls, 3);

        bool match = false;
        {
            PokemonStats stats = generate(seed, rolls, 0);
            if (matches(m_stats, stats)){
                match = true;
                print(seed, rolls, 0);
            }
        }
        {
            PokemonStats stats = generate(seed, rolls, 3);
            if (matches(m_stats, stats)){
                match = true;
                print(seed, rolls, 3);
            }
        }
        if (match){
            m_seeds.insert(seed);
        }
    }

private:
    const PokemonStats& m_stats;
    std::mutex m_lock;
    std::set<uint64_t> m_seeds;
};





int main(){











    std::cout << "PLA Seed Finder" << std::endl;
    std::cout << std::endl;
    std::cout << "Pokemon Automation Discord Server (https://discord.gg/cQ4gWxN)" << std::endl;
    std::cout << "GitHub: TBD" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Enter your Pokemon's stats. Do not choose a shiny!" << std::endl;
    std::cout << std::endl;

    PokemonStats stats;

    {
        std::cout << "Enter the PID as hex (0123abCD): ";
        std::string str;
        std::getline(std::cin, str);
        stats.pid = parse_hex32(str);
    }
    {
        std::cout << "Enter the EC as hex (0123abCD): ";
        std::string str;
        std::getline(std::cin, str);
        stats.ec = parse_hex32(str);
    }
    std::cout << std::endl;
    {
        std::cout << "Enter IVs (31 31 -1 61 15 6): (-1 ignores that IV)" << std::endl;
        std::cout << "IVs: ";
        std::string str;
        std::getline(std::cin, str);
        const char* ptr = str.c_str();
        for (size_t c = 0; c < 6; c++){
            skip_whitespace(ptr);
            stats.ivs[c] = parse_int(ptr);
        }
    }

    size_t rolls = 32;
    size_t threads = std::thread::hardware_concurrency();

    std::cout << std::endl;
    print_isa();
    std::cout << "Threads: " << threads << std::endl;
    std::cout << std::endl;

    std::cout << "Searching..." << std::endl;
    std::cout << std::endl;

    EcPidMatchReporter reporter(stats);

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    {
        DynamicParallelizer session(
            [&](uint64_t start, uint64_t length){
                report_ec_pid_matches(reporter, rolls, stats.pid, (uint32_t)(stats.ec - 0x82A2B175229D6A5B) + (start << 32), length);
            },
            65536, 0x100000000, threads
        );
    }

    std::cout << std::endl;
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

    cout << "seconds = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000. << endl;


}

