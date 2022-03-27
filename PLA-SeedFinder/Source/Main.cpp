
#include <functional>
#include <chrono>
#include <vector>
#include <set>
#include <sstream>
#include "SeedScan.h"
#include "Kernels/XoroShiro1_Default.h"
#include "ReportCandidates.h"
#include "DynamicParallelizer.h"
#include "Tools.h"
#include "PLA-SeedFinder.h"

#include <iostream>
using std::cout;
using std::endl;

using namespace PokemonAutomation;




int main(){

    std::cout << "PLA Seed Finder" << std::endl;
    std::cout << std::endl;
    std::cout << "Pokemon Automation Discord Server (https://discord.gg/cQ4gWxN)" << std::endl;
    std::cout << "GitHub: https://github.com/PokemonAutomation/Experimental/blob/main/PLA-SeedFinder" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Enter your Pokemon's stats. Do not choose a shiny!" << std::endl;
    std::cout << std::endl;

//    PokemonStats stats;

    uint32_t pid, ec;
    int8_t ivs[6];
    {
        std::cout << "Enter the PID as hex (0123abCD): ";
        std::string str;
        std::getline(std::cin, str);
        pid = parse_hex32(str);
    }
    {
        std::cout << "Enter the EC as hex (0123abCD): ";
        std::string str;
        std::getline(std::cin, str);
        ec = parse_hex32(str);
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
            ivs[c] = parse_int(ptr);
        }
    }

    uint8_t max_rolls = 32;
    size_t threads = std::thread::hardware_concurrency();

#if 0
    std::cout << std::endl;
    print_isa();
    std::cout << "Threads: " << threads << std::endl;
    std::cout << std::endl;

    std::cout << "Searching..." << std::endl;
    std::cout << std::endl;

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    run_search(stats, rolls, threads);

    std::cout << std::endl;
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

    cout << "seconds = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000. << endl;
#endif
    cout << endl;

    const uint32_t BUFFER_SIZE = 16;
    uint64_t seeds[BUFFER_SIZE];
    uint8_t rolls[BUFFER_SIZE];
    pa_PLA_find_seeds(pid, ec, ivs, max_rolls, seeds, rolls, BUFFER_SIZE);


#if 0
    if (count < 0){
        count = BUFFER_SIZE;
    }

    cout << "Total Found: " << count << endl;
    for (size_t c = 0; c < count; c++){
        cout << tostr_hex_padded(seeds[c], 16) << " : " << (int)rolls[c] << endl;
    }
#endif

    system("pause");
}



