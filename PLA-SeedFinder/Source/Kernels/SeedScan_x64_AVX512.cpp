/*  Seed Scan (AVX512)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <stdint.h>
#include "XoroShiro8_AVX512.h"

namespace PokemonAutomation{


bool seed_scan_unroll16_AVX512(size_t rolls, uint32_t desired_pid, uint64_t start_seed, uint64_t iterations){
    __m512i vpid = _mm512_set1_epi64(desired_pid);

    __m512i seed0 = _mm512_add_epi64(_mm512_set1_epi64(start_seed), _mm512_setr_epi32(0,  0, 0,  1, 0,  2, 0,  3, 0,  4, 0,  5, 0,  6, 0,  7));
    __m512i seed1 = _mm512_add_epi64(_mm512_set1_epi64(start_seed), _mm512_setr_epi32(0,  8, 0,  9, 0, 10, 0, 11, 0, 12, 0, 13, 0, 14, 0, 15));

    __mmask16 diff = 0;

    iterations /= 16;
    do{
        XoroShiroX8 rng0(seed0);
        XoroShiroX8 rng1(seed1);
        rng0.next();
        rng1.next();

        __m512i pid0, pid1;
        size_t lc = rolls;
        do{
            rng0.next();
            rng1.next();
            pid0 = rng0.get_int64();
            pid1 = rng1.get_int64();
            diff |= _mm512_cmpeq_epi32_mask(pid0, vpid);
            diff |= _mm512_cmpeq_epi32_mask(pid1, vpid);
        }while (--lc);

        seed0 = _mm512_add_epi64(seed0, _mm512_set1_epi64(0x1000000000));
        seed1 = _mm512_add_epi64(seed1, _mm512_set1_epi64(0x1000000000));
    }while (--iterations);

    return diff & 0x5555;
}




}
