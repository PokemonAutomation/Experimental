/*  Tools
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#ifndef PokemonAutomation_SeedFinder_PokemonLA_Tools_H
#define PokemonAutomation_SeedFinder_PokemonLA_Tools_H

#include <stdint.h>
#include <string>

namespace PokemonAutomation{


std::string tostr_hex_padded(uint64_t x, size_t length);
uint32_t parse_hex32(std::string str);

void skip_whitespace(const char*& stream);
int parse_int(const char*& stream);



struct PokemonStats{
    uint32_t pid;
    uint32_t ec;
    int ivs[6] = {-1, -1, -1, -1, -1, -1};
};

PokemonStats generate(uint64_t seed, size_t rolls, int max_ivs);
bool matches(const PokemonStats& filter, const PokemonStats& stats);
void print(uint64_t seed, size_t rolls, int max_ivs);



}
#endif
