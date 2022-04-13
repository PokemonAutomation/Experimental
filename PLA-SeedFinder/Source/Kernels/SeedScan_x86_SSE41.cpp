/*  Seed Scan (x86 SSE4.1)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <stdint.h>
#include <smmintrin.h>
#include "XoroShiro2_x86_SSE2.h"

namespace PokemonAutomation{


bool seed_scan_common_unroll4_SSE41(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    __m128i vpid = _mm_set1_epi32(desired_pid);

    __m128i seed0 = _mm_add_epi64(_mm_set1_epi64x(start_seed), _mm_setr_epi32(0, 0, 0, 1));
    __m128i seed1 = _mm_add_epi64(_mm_set1_epi64x(start_seed), _mm_setr_epi32(0, 2, 0, 3));

    __m128i diff0 = _mm_setzero_si128();

    iterations /= 4;
    do{
        XoroShiroX2_x86_SSE41 rng0(seed0);
        XoroShiroX2_x86_SSE41 rng1(seed1);
        rng0.next();
        rng1.next();

        __m128i pid0, pid1;
        size_t lc = rolls;
        do{
            rng0.next();
            rng1.next();
            pid0 = rng0.get_int64();
            pid1 = rng1.get_int64();
            pid0 = _mm_blend_epi16(pid0, _mm_shuffle_epi32(pid1, 177), 0xcc);
            pid0 = _mm_cmpeq_epi32(pid0, vpid);
            diff0 = _mm_or_si128(diff0, pid0);
        }while (--lc);

        seed0 = _mm_add_epi64(seed0, _mm_set1_epi64x(0x400000000));
        seed1 = _mm_add_epi64(seed1, _mm_set1_epi64x(0x400000000));
    }while (--iterations);

    return !_mm_test_all_zeros(diff0, diff0);
}
bool seed_scan_thorough_unroll4_SSE41(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    __m128i vpid = _mm_set1_epi32(desired_pid & 0xefffffff);

    __m128i seed0 = _mm_add_epi64(_mm_set1_epi64x(start_seed), _mm_setr_epi32(0, 0, 0, 1));
    __m128i seed1 = _mm_add_epi64(_mm_set1_epi64x(start_seed), _mm_setr_epi32(0, 2, 0, 3));

    __m128i diff0 = _mm_setzero_si128();

    iterations /= 4;
    do{
        XoroShiroX2_x86_SSE41 rng0(seed0);
        XoroShiroX2_x86_SSE41 rng1(seed1);
//        rng0.next();
//        rng1.next();

        __m128i pid0, pid1;
        size_t lc = rolls + 1;
        do{
            rng0.next();
            rng1.next();
            pid0 = rng0.get_int64();
            pid1 = rng1.get_int64();
            pid0 = _mm_blend_epi16(pid0, _mm_shuffle_epi32(pid1, 177), 0xcc);
            diff0 = _mm_or_si128(diff0, _mm_cmpeq_epi32(pid0, _mm_set1_epi32(-1)));
            pid0 = _mm_and_si128(pid0, _mm_set1_epi32(0xefffffff));
            diff0 = _mm_or_si128(diff0, _mm_cmpeq_epi32(pid0, vpid));
        }while (--lc);

        seed0 = _mm_add_epi64(seed0, _mm_set1_epi64x(0x400000000));
        seed1 = _mm_add_epi64(seed1, _mm_set1_epi64x(0x400000000));
    }while (--iterations);

    return !_mm_test_all_zeros(diff0, diff0);
}




}
