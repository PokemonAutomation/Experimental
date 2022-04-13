/*  XoroShiro x4 (x86 AVX2)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#ifndef PokemonAutomation_XoroShiro4_x86_AVX2_H
#define PokemonAutomation_XoroShiro4_x86_AVX2_H

#include <immintrin.h>
#include "Compiler.h"

namespace PokemonAutomation{

class XoroShiroX4_x86_AVX2{
public:
    PA_FORCE_INLINE XoroShiroX4_x86_AVX2(__m256i seed){
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



#if 0
class XoroShiroX8_x86_AVX2{
public:
    PA_FORCE_INLINE XoroShiroX8_x86_AVX2(__m256i seed0, __m256i seed1){
        state0L = _mm256_blend_epi16(seed0, _mm256_shuffle_epi32(seed1, 177), 0xcc);
        state0H = _mm256_blend_epi16(_mm256_shuffle_epi32(seed0, 177), seed1, 0xcc);
        state1L = _mm256_set1_epi32(0x229D6A5B);
        state1H = _mm256_set1_epi32(0x82A2B175);
    }
    PA_FORCE_INLINE __m256i get_int32(){
        return _mm256_add_epi32(state0L, state1L);
    }
    PA_FORCE_INLINE void next(){
        __m256i s0L = state0L;
        __m256i s0H = state0H;
        __m256i s1L = state1L;
        __m256i s1H = state1H;

        s1L = _mm256_xor_si256(s1L, state0L);
        s1H = _mm256_xor_si256(s1H, state0H);

        s0L = _mm256_or_si256(_mm256_slli_epi32(state0L, 24), _mm256_srli_epi32(state0H, 8));
        s0H = _mm256_or_si256(_mm256_slli_epi32(state0H, 24), _mm256_srli_epi32(state0L, 8));

        state1L = _mm256_or_si256(_mm256_srli_epi32(s1L, 27), _mm256_slli_epi32(s1H, 5));
        state1H = _mm256_or_si256(_mm256_srli_epi32(s1H, 27), _mm256_slli_epi32(s1L, 5));

        state0L = _mm256_slli_epi32(state1L, 16);
        state0H = _mm256_or_si256(_mm256_slli_epi32(state1H, 16), _mm256_srli_epi32(state1L, 16));

        state0L = _mm256_xor_si256(state0L, s1L);
        state0H = _mm256_xor_si256(state0H, s1H);

        state0L = _mm256_xor_si256(state0L, s0L);
        state0H = _mm256_xor_si256(state0H, s0H);
    }

private:
    __m256i state0L;
    __m256i state0H;
    __m256i state1L;
    __m256i state1H;
};
#endif



}
#endif
