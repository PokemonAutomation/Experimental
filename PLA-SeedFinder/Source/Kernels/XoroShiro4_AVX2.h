/*  XoroShiro x4 (AVX2)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#pragma once
#ifndef PokemonAutomation_XoroShiro4_AVX2_H
#define PokemonAutomation_XoroShiro4_AVX2_H

#include <immintrin.h>
#include "Compiler.h"

namespace PokemonAutomation{

class XoroShiroX4{
public:
    PA_FORCE_INLINE XoroShiroX4(__m256i seed){
        state[0] = seed;
        state[1] = _mm256_set1_epi64x(0x82A2B175229D6A5B);
    }

    PA_FORCE_INLINE __m256i get_int64(){
        return _mm256_add_epi64(state[0], state[1]);
    }
    PA_FORCE_INLINE __m256i get_int32(){
        return _mm256_and_si256(get_int64(), _mm256_set1_epi64x(0x00000000ffffffff));
    }
    PA_FORCE_INLINE void next(){
        __m256i s0 = state[0];
        __m256i s1 = state[1];
        s1 = _mm256_xor_si256(s1, s0);

        s0 = _mm256_or_si256(_mm256_slli_epi64(s0, 24), _mm256_srli_epi64(s0, 40));
        state[1] = _mm256_or_si256(_mm256_slli_epi64(s1, 37), _mm256_srli_epi64(s1, 27));

        s1 = _mm256_xor_si256(s1, _mm256_slli_epi64(s1, 16));
        state[0] = _mm256_xor_si256(s0, s1);
    }

private:
    __m256i state[2];
};


}
#endif
