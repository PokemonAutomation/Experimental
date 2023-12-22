#pragma once

#if defined __aarch64__ && defined __APPLE__
#include <simd/simd.h>
#include "Compiler.h"

#define MAGIC_NUMBER 0x82A2B175229D6A5B

namespace PokemonAutomation{

static const simd_ulong4 magic_ulong4 = simd_make_ulong4(MAGIC_NUMBER, MAGIC_NUMBER, MAGIC_NUMBER, MAGIC_NUMBER);
static const simd_ulong4 mask_ulong4 = simd_make_ulong4(UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX);
static const simd_ulong8 mask_ulong8 = simd_make_ulong8(simd_make_ulong4(UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX),
                                                        simd_make_ulong4(UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX));

class XoroShiro4{
public:
    PA_FORCE_INLINE XoroShiro4(simd_ulong4 seed)
    : state {seed, magic_ulong4} {}
    PA_FORCE_INLINE simd_ulong4 get(){ return state[0] + state[1]; }
    PA_FORCE_INLINE simd_ulong4 get_masked(){ return (state[0] + state[1]) & mask_ulong4; }
    PA_FORCE_INLINE void next(){
        simd_ulong4 s0 = state[0];
        simd_ulong4 s1 = state[1];        
        s1 = s1 ^ s0;
        s0 = (s0 << 24) | (s0 >> 40);
        state[1] = (s1 << 37) | (s1 >> 27);
        s1 = s1 ^ (s1 << 16);
        state[0] = s0 ^ s1;
    }

private:
    simd_ulong4 state[2];
};


class XoroShiro8{
public:
    PA_FORCE_INLINE XoroShiro8(simd_ulong8 seed)
    : state {seed, simd_make_ulong8(magic_ulong4, magic_ulong4)} {}
    PA_FORCE_INLINE simd_ulong8 get(){ return state[0] + state[1]; }
    PA_FORCE_INLINE simd_ulong8 get_masked(){ return (state[0] + state[1]) & mask_ulong8; }
    PA_FORCE_INLINE void next(){
        simd_ulong8 s0 = state[0];
        simd_ulong8 s1 = state[1];        
        s1 = s1 ^ s0;
        s0 = (s0 << 24) | (s0 >> 40);
        state[1] = (s1 << 37) | (s1 >> 27);
        s1 = s1 ^ (s1 << 16);
        state[0] = s0 ^ s1;
    }

private:
    simd_ulong8 state[2];
};


}

#undef MAGIC_NUMBER
#endif
