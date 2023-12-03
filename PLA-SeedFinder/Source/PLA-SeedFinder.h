/*  PLA SeedFinder
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <stdint.h>

#if defined(_WIN32)
#if defined _WINDLL
#define PA_EXPORT __declspec(dllexport)
#elif defined PA_STATIC
#define PA_EXPORT
#else
#define PA_EXPORT __declspec(dllimport)
#endif
#else
#define PA_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif



//  Find the seed for the specified Pokemon.
//
//  Returns the # of matching seeds.
//
//  Seeds and their associated # of rolls are returned into "seeds" and "rolls".
//  "length" specifies the buffer size of both "seeds" and "rolls".
//
//  If there are more matches than can fit in the buffer, it will store as many
//  as possible and return -1.
PA_EXPORT int32_t pa_PLA_find_seeds(
    uint32_t pid, uint32_t ec, int8_t ivs[6], uint8_t max_rolls,
    uint64_t* seeds, uint8_t* rolls, uint32_t length
);

//  Same as above, but lets you select the # of threads.
PA_EXPORT int32_t pa_PLA_find_seeds_threads(
    uint32_t threads,
    uint32_t pid, uint32_t ec, int8_t ivs[6], uint8_t max_rolls,
    uint64_t* seeds, uint8_t* rolls, uint32_t length
);




#ifdef __cplusplus
}
#endif
