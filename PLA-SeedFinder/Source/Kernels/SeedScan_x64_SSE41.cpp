/*  Seed Scan (SSE4.1)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <stdint.h>
#include <smmintrin.h>
#include "XoroShiro2_SSE2.h"

namespace PokemonAutomation{


bool seed_scan_normalPID_unroll4_SSE41(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    __m128i vpid = _mm_set1_epi64x(desired_pid & 0xefffffff);

    __m128i seed0 = _mm_add_epi64(_mm_set1_epi64x(start_seed), _mm_setr_epi32(0, 0, 0, 1));
    __m128i seed1 = _mm_add_epi64(_mm_set1_epi64x(start_seed), _mm_setr_epi32(0, 2, 0, 3));

    __m128i diff0 = _mm_setzero_si128();
    __m128i diff1 = _mm_setzero_si128();

    iterations /= 4;
    do{
        XoroShiroX2 rng0(seed0);
        XoroShiroX2 rng1(seed1);
        rng0.next();
        rng1.next();

        __m128i pid0, pid1;
        size_t lc = rolls;
        do{
            rng0.next();
            rng1.next();
            pid0 = rng0.get_int64();
            pid1 = rng1.get_int64();
            pid0 = _mm_and_si128(pid0, _mm_set1_epi64x(0xefffffff));
            pid1 = _mm_and_si128(pid1, _mm_set1_epi64x(0xefffffff));
            pid0 = _mm_cmpeq_epi32(pid0, vpid);
            pid1 = _mm_cmpeq_epi32(pid1, vpid);
            diff0 = _mm_or_si128(diff0, pid0);
            diff1 = _mm_or_si128(diff1, pid1);
        }while (--lc);

        seed0 = _mm_add_epi64(seed0, _mm_set1_epi64x(0x400000000));
        seed1 = _mm_add_epi64(seed1, _mm_set1_epi64x(0x400000000));
    }while (--iterations);

    diff0 = _mm_or_si128(diff0, diff1);
    return !_mm_test_all_zeros(diff0, _mm_set1_epi64x(0x00000000ffffffff));
}
bool seed_scan_maxPID_unroll4_SSE41(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    __m128i vpid = _mm_set1_epi64x(desired_pid & 0xefffffff);

    __m128i seed0 = _mm_add_epi64(_mm_set1_epi64x(start_seed), _mm_setr_epi32(0, 0, 0, 1));
    __m128i seed1 = _mm_add_epi64(_mm_set1_epi64x(start_seed), _mm_setr_epi32(0, 2, 0, 3));

    __m128i diff0 = _mm_setzero_si128();
    __m128i diff1 = _mm_setzero_si128();

    iterations /= 4;
    do{
        XoroShiroX2 rng0(seed0);
        XoroShiroX2 rng1(seed1);
//        rng0.next();
//        rng1.next();

        __m128i pid0, pid1;
        size_t lc = rolls + 1;
        do{
            rng0.next();
            rng1.next();
            pid0 = rng0.get_int64();
            pid1 = rng1.get_int64();
            diff0 = _mm_or_si128(diff0, _mm_cmpeq_epi32(pid0, _mm_set1_epi32(-1)));
            diff1 = _mm_or_si128(diff1, _mm_cmpeq_epi32(pid1, _mm_set1_epi32(-1)));
            pid0 = _mm_and_si128(pid0, _mm_set1_epi64x(0xefffffff));
            pid1 = _mm_and_si128(pid1, _mm_set1_epi64x(0xefffffff));
            diff0 = _mm_or_si128(diff0, _mm_cmpeq_epi32(pid0, vpid));
            diff1 = _mm_or_si128(diff1, _mm_cmpeq_epi32(pid1, vpid));
        }while (--lc);

        seed0 = _mm_add_epi64(seed0, _mm_set1_epi64x(0x400000000));
        seed1 = _mm_add_epi64(seed1, _mm_set1_epi64x(0x400000000));
    }while (--iterations);

    diff0 = _mm_or_si128(diff0, diff1);
    return !_mm_test_all_zeros(diff0, _mm_set1_epi64x(0x00000000ffffffff));
}




}
