/*  Seed Scan (AVX2)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <stdint.h>
#include "XoroShiro4_AVX2.h"

namespace PokemonAutomation{


bool seed_scan_normalPID_unroll8_AVX2(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    __m256i vpid = _mm256_set1_epi64x(desired_pid & 0xefffffff);

    __m256i seed0 = _mm256_add_epi64(_mm256_set1_epi64x(start_seed), _mm256_setr_epi32(0, 0, 0, 1, 0, 2, 0, 3));
    __m256i seed1 = _mm256_add_epi64(_mm256_set1_epi64x(start_seed), _mm256_setr_epi32(0, 4, 0, 5, 0, 6, 0, 7));

    __m256i diff0 = _mm256_setzero_si256();
    __m256i diff1 = _mm256_setzero_si256();

    iterations /= 8;
    do{
        XoroShiroX4 rng0(seed0);
        XoroShiroX4 rng1(seed1);
        rng0.next();
        rng1.next();

        __m256i pid0, pid1;
        size_t lc = rolls;
        do{
            rng0.next();
            rng1.next();
            pid0 = rng0.get_int64();
            pid1 = rng1.get_int64();
            pid0 = _mm256_and_si256(pid0, _mm256_set1_epi64x(0xefffffff));
            pid1 = _mm256_and_si256(pid1, _mm256_set1_epi64x(0xefffffff));
            pid0 = _mm256_cmpeq_epi32(pid0, vpid);
            pid1 = _mm256_cmpeq_epi32(pid1, vpid);
            diff0 = _mm256_or_si256(diff0, pid0);
            diff1 = _mm256_or_si256(diff1, pid1);
        }while (--lc);

        seed0 = _mm256_add_epi64(seed0, _mm256_set1_epi64x(0x800000000));
        seed1 = _mm256_add_epi64(seed1, _mm256_set1_epi64x(0x800000000));
    }while (--iterations);

    diff0 = _mm256_or_si256(diff0, diff1);
    return !_mm256_testz_si256(diff0, _mm256_set1_epi64x(0x00000000ffffffff));
}
bool seed_scan_maxPID_unroll8_AVX2(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    __m256i vpid = _mm256_set1_epi64x(desired_pid & 0xefffffff);

    __m256i seed0 = _mm256_add_epi64(_mm256_set1_epi64x(start_seed), _mm256_setr_epi32(0, 0, 0, 1, 0, 2, 0, 3));
    __m256i seed1 = _mm256_add_epi64(_mm256_set1_epi64x(start_seed), _mm256_setr_epi32(0, 4, 0, 5, 0, 6, 0, 7));

    __m256i diff0 = _mm256_setzero_si256();
    __m256i diff1 = _mm256_setzero_si256();

    iterations /= 8;
    do{
        XoroShiroX4 rng0(seed0);
        XoroShiroX4 rng1(seed1);
//        rng0.next();
//        rng1.next();

        __m256i pid0, pid1;
        size_t lc = rolls + 1;
        do{
            rng0.next();
            rng1.next();
            pid0 = rng0.get_int64();
            pid1 = rng1.get_int64();
            diff0 = _mm256_or_si256(diff0, _mm256_cmpeq_epi32(pid0, _mm256_set1_epi32(-1)));
            diff1 = _mm256_or_si256(diff1, _mm256_cmpeq_epi32(pid1, _mm256_set1_epi32(-1)));
            pid0 = _mm256_and_si256(pid0, _mm256_set1_epi64x(0xefffffff));
            pid1 = _mm256_and_si256(pid1, _mm256_set1_epi64x(0xefffffff));
            diff0 = _mm256_or_si256(diff0, _mm256_cmpeq_epi32(pid0, vpid));
            diff1 = _mm256_or_si256(diff1, _mm256_cmpeq_epi32(pid1, vpid));
        }while (--lc);

        seed0 = _mm256_add_epi64(seed0, _mm256_set1_epi64x(0x800000000));
        seed1 = _mm256_add_epi64(seed1, _mm256_set1_epi64x(0x800000000));
    }while (--iterations);

    diff0 = _mm256_or_si256(diff0, diff1);
    return !_mm256_testz_si256(diff0, _mm256_set1_epi64x(0x00000000ffffffff));
}




}
