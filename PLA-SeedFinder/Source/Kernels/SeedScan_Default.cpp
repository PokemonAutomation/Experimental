/*  Seed Scan (Default)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <stdint.h>
#include <cstddef>
#include "XoroShiro1_Default.h"

namespace PokemonAutomation{


bool seed_scan_Default(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    uint64_t seed0 = start_seed;

    bool match = false;
    desired_pid &= 0xefffffff;

    do{
        XoroShiroX1 rng0(seed0);
        rng0.next();

        uint32_t pid0;
        size_t lc = rolls;
        do{
            rng0.next();
            pid0 = rng0.get_int32() & 0xefffffff;
            match |= pid0 == desired_pid;
        }while (--lc);

        seed0 += 0x100000000;
    }while (--iterations);

    return match;
}




}
