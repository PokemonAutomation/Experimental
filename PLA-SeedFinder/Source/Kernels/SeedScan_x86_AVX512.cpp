/*  Seed Scan (x86 AVX512)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <stdint.h>
#include "XoroShiro8_x86_AVX512.h"

namespace PokemonAutomation{


bool seed_scan_common_unroll16_AVX512(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    __m512i vpid = _mm512_set1_epi32(desired_pid);

    __m512i seed0 = _mm512_add_epi64(_mm512_set1_epi64(start_seed), _mm512_setr_epi32(0,  0, 0,  1, 0,  2, 0,  3, 0,  4, 0,  5, 0,  6, 0,  7));
    __m512i seed1 = _mm512_add_epi64(_mm512_set1_epi64(start_seed), _mm512_setr_epi32(0,  8, 0,  9, 0, 10, 0, 11, 0, 12, 0, 13, 0, 14, 0, 15));

    __mmask16 diff = 0xffff;

    iterations /= 16;
    do{
        XoroShiroX8_x86_AVX512 rng0(seed0);
        XoroShiroX8_x86_AVX512 rng1(seed1);
        rng0.next();
        rng1.next();

        __m512i pid0, pid1;
        size_t lc = rolls;
        do{
            rng0.next();
            rng1.next();
            pid0 = rng0.get_int64();
            pid1 = rng1.get_int64();
//            pid0 = _mm512_mask_shuffle_epi32(pid0, 0xaaaa, pid1, _MM_PERM_CDAB);
            diff = _mm512_mask_cmpneq_epi32_mask(diff, pid0, vpid);
            diff = _mm512_mask_cmpneq_epi32_mask(diff, pid1, vpid);
        }while (--lc);

        seed0 = _mm512_add_epi64(seed0, _mm512_set1_epi64(0x1000000000));
        seed1 = _mm512_add_epi64(seed1, _mm512_set1_epi64(0x1000000000));
    }while (--iterations);

    return (diff & 0x5555) != 0x5555;
}
bool seed_scan_thorough_unroll16_AVX512(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    __m512i vpid = _mm512_set1_epi32(desired_pid & 0xefffffff);

    __m512i seed0 = _mm512_add_epi64(_mm512_set1_epi64(start_seed), _mm512_setr_epi32(0,  0, 0,  1, 0,  2, 0,  3, 0,  4, 0,  5, 0,  6, 0,  7));
    __m512i seed1 = _mm512_add_epi64(_mm512_set1_epi64(start_seed), _mm512_setr_epi32(0,  8, 0,  9, 0, 10, 0, 11, 0, 12, 0, 13, 0, 14, 0, 15));

    __mmask16 diff = 0xffff;

    iterations /= 16;
    do{
        XoroShiroX8_x86_AVX512 rng0(seed0);
        XoroShiroX8_x86_AVX512 rng1(seed1);
//        rng0.next();
//        rng1.next();

        __m512i pid0, pid1;
        size_t lc = rolls + 1;
        do{
            rng0.next();
            rng1.next();
            pid0 = rng0.get_int64();
            pid1 = rng1.get_int64();
            pid0 = _mm512_mask_shuffle_epi32(pid0, 0xaaaa, pid1, _MM_PERM_CDAB);
            diff = _mm512_mask_cmpneq_epi32_mask(diff, pid0, _mm512_set1_epi32(-1));
            pid0 = _mm512_and_si512(pid0, _mm512_set1_epi32(0xefffffff));
            diff = _mm512_mask_cmpneq_epi32_mask(diff, pid0, vpid);
        }while (--lc);

        seed0 = _mm512_add_epi64(seed0, _mm512_set1_epi64(0x1000000000));
        seed1 = _mm512_add_epi64(seed1, _mm512_set1_epi64(0x1000000000));
    }while (--iterations);

    return diff != 0xffff;
}




}
