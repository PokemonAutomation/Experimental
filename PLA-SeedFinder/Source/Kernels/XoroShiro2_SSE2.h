/*  XoroShiro x2 (SSE2)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#pragma once
#ifndef PokemonAutomation_XoroShiro2_SSE2_H
#define PokemonAutomation_XoroShiro2_SSE2_H

#include <emmintrin.h>
#include "Compiler.h"

namespace PokemonAutomation{

class XoroShiroX2{
public:
    PA_FORCE_INLINE XoroShiroX2(__m128i seed){
        state[0] = seed;
        state[1] = _mm_set1_epi64x(0x82A2B175229D6A5B);
    }

    PA_FORCE_INLINE __m128i get_int64(){
        return _mm_add_epi64(state[0], state[1]);
    }
    PA_FORCE_INLINE __m128i get_int32(){
        return _mm_and_si128(get_int64(), _mm_set1_epi64x(0x00000000ffffffff));
    }
    PA_FORCE_INLINE void next(){
        __m128i s0 = state[0];
        __m128i s1 = state[1];
        s1 = _mm_xor_si128(s1, s0);

        s0 = _mm_or_si128(_mm_slli_epi64(s0, 24), _mm_srli_epi64(s0, 40));
        state[1] = _mm_or_si128(_mm_slli_epi64(s1, 37), _mm_srli_epi64(s1, 27));

        s1 = _mm_xor_si128(s1, _mm_slli_epi64(s1, 16));
        state[0] = _mm_xor_si128(s0, s1);
    }

private:
    __m128i state[2];
};


}
#endif
