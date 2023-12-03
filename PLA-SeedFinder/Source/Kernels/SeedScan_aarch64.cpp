#include <stdint.h>
#include <cstddef>
#include "XoroShiro_aarch64.h"

namespace PokemonAutomation{

#if defined __aarch64__ && defined __APPLE__

bool seed_scan_common_unroll4(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    simd_ulong4 t = simd_make_ulong4((uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid);
    simd_ulong4 seed = simd_make_ulong4(start_seed, start_seed+0x200000000, start_seed+0x300000000, start_seed+0x400000000);
    simd_ulong4 delta = simd_make_ulong4(0x500000000, 0x500000000, 0x500000000, 0x500000000);
    simd_ulong4 pid;
    iterations /= 4;

    do {
        XoroShiro4 rng(seed);
        rng.next();
        size_t lc = rolls;
        do {
            rng.next();
            pid = rng.get_masked();
            if (simd_reduce_min(pid ^ t) == 0) {
                return true;
            }
        } while (--lc);
        seed += delta;
    } while (--iterations);
    return false;
}

bool seed_scan_thorough_unroll4(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    desired_pid &= 0xefffffff;
    simd_ulong4 t = simd_make_ulong4((uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid);
    simd_ulong4 seed = simd_make_ulong4(start_seed, start_seed+0x200000000, start_seed+0x300000000, start_seed+0x400000000);
    simd_ulong4 delta = simd_make_ulong4(0x500000000, 0x500000000, 0x500000000, 0x500000000);
    simd_ulong4 mask = simd_make_ulong4(0xefffffff, 0xefffffff, 0xefffffff, 0xefffffff);
    simd_ulong4 pid;
    iterations /= 4;

    do {
        XoroShiro4 rng(seed);
        size_t lc = rolls + 1;
        do {
            rng.next();
            pid = rng.get_masked();
            if (simd_reduce_max(pid) == UINT32_MAX) {
                return true;
            }
            pid &= mask;
            if (simd_reduce_min(pid ^ t) == 0) {
                return true;
            }
        } while (--lc);
        seed += delta;
    } while (--iterations);
    return false;
}

bool seed_scan_common_unroll8(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    simd_ulong8 t = simd_make_ulong8(simd_make_ulong4((uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid),
                                     simd_make_ulong4((uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid));
    simd_ulong8 seed = simd_make_ulong8(simd_make_ulong4(start_seed, start_seed+0x200000000, start_seed+0x300000000, start_seed+0x400000000),
                                        simd_make_ulong4(start_seed+0x500000000, start_seed+0x600000000, start_seed+0x700000000, start_seed+0x800000000));
    simd_ulong8 delta = simd_make_ulong8(simd_make_ulong4(0x900000000, 0x900000000, 0x900000000, 0x900000000),
                                         simd_make_ulong4(0x900000000, 0x900000000, 0x900000000, 0x900000000));
    simd_ulong8 pid;
    iterations /= 8;

    do {
        XoroShiro8 rng(seed);
        rng.next();
        size_t lc = rolls;
        do {
            rng.next();
            pid = rng.get_masked();
            if (simd_reduce_min(pid ^ t) == 0) {
                return true;
            }
        } while (--lc);
        seed += delta;
    } while (--iterations);
    return false;
}

bool seed_scan_thorough_unroll8(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    desired_pid &= 0xefffffff;
    simd_ulong8 t = simd_make_ulong8(simd_make_ulong4((uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid),
                                     simd_make_ulong4((uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid, (uint64_t)desired_pid));
    simd_ulong8 seed = simd_make_ulong8(simd_make_ulong4(start_seed, start_seed+0x200000000, start_seed+0x300000000, start_seed+0x400000000),
                                        simd_make_ulong4(start_seed+0x500000000, start_seed+0x600000000, start_seed+0x700000000, start_seed+0x800000000));
    simd_ulong8 delta = simd_make_ulong8(simd_make_ulong4(0x900000000, 0x900000000, 0x900000000, 0x900000000),
                                         simd_make_ulong4(0x900000000, 0x900000000, 0x900000000, 0x900000000));
    simd_ulong8 u1 = simd_make_ulong8(simd_make_ulong4(0xefffffff, 0xefffffff, 0xefffffff, 0xefffffff), 
                                     simd_make_ulong4(0xefffffff, 0xefffffff, 0xefffffff, 0xefffffff));
    simd_ulong8 pid;
    iterations /= 8;

    do {
        XoroShiro8 rng(seed);
        size_t lc = rolls + 1;
        do {
            rng.next();
            pid = rng.get_masked();
            if (simd_reduce_max(pid) == UINT32_MAX) {
                return true;
            }
            pid &= u1;
            if (simd_reduce_min(pid ^ t) == 0) {
                return true;
            }
        } while (--lc);
        seed += delta;
    } while (--iterations);
    return false;
}

#endif
}
