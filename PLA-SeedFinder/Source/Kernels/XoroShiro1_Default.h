/*  XoroShiro x1 (Default)
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#ifndef PokemonAutomation_XoroShiro1_Default_H
#define PokemonAutomation_XoroShiro1_Default_H

#include <stdint.h>
#include "Compiler.h"

namespace PokemonAutomation{

class XoroShiroX1{
public:
    PA_FORCE_INLINE XoroShiroX1(uint64_t seed){
        state[0] = seed;
        state[1] = 0x82A2B175229D6A5B;
    }

    PA_FORCE_INLINE uint64_t get_int64(){
        return state[0] + state[1];
    }
    PA_FORCE_INLINE uint32_t get_int32(){
        return (uint32_t)get_int64();
    }
    PA_FORCE_INLINE void next(){
        uint64_t s0 = state[0];
        uint64_t s1 = state[1];
        s1 = s1 ^ s0;

        s0 = (s0 << 24) | (s0 >> 40);
        state[1] = (s1 << 37) | (s1 >> 27);

        s1 = s1 ^ (s1 << 16);
        state[0] = s0 ^ s1;
    }

    uint32_t next32(uint32_t mod, uint32_t mask){
        uint32_t ret;
        do{
            ret = get_int32() & mask;
            next();
        }while (ret >= mod);
        return ret;
    }

private:
    uint64_t state[2];
};


}
#endif
