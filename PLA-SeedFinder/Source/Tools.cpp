/*  Tools
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include <sstream>
#include "Kernels/XoroShiro1_Default.h"
#include "Tools.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{


std::mutex print_lock;



std::string tostr_hex_padded(uint64_t x, size_t length){
    std::stringstream ss;
    ss << std::hex << x << std::dec;
    std::string str = ss.str();
    if (str.size() < length){
        str = std::string(length - str.size(), '0') + str;
    }
    return str;
}
uint32_t parse_hex32(std::string str){
    uint32_t ret = 0;
    const char* ptr = str.c_str();
    while (true){
        char ch = *ptr++;
        if ('0' <= ch && ch <= '9'){
            ret *= 16;
            ret += ch - '0';
            continue;
        }
        if ('a' <= ch && ch <= 'f'){
            ret *= 16;
            ret += ch - 'a' + 10;
            continue;
        }
        if ('A' <= ch && ch <= 'F'){
            ret *= 16;
            ret += ch - 'A' + 10;
            continue;
        }
        break;
    }
    return ret;
}


void skip_whitespace(const char*& stream){
    while (true){
        char ch = stream[0];
        if (ch != ' ' && ch != '\t'){
            break;
        }
        stream++;
    }
}
int parse_int(const char*& stream){
    if (stream[0] == '-'){
        stream++;
        return -parse_int(stream);
    }
    int ret = 0;
    while (true){
        char ch = stream[0];
        if (!('0' <= ch && ch <= '9')){
            break;
        }
        ret *= 10;
        ret += ch - '0';
        stream++;
    }
    return ret;
}






PokemonStats generate(uint64_t seed, size_t rolls, int max_ivs){
    XoroShiroX1_Default rng(seed);

    PokemonStats stats;

    stats.ec = rng.get_int32_next(0xffffffff, 0xffffffff);

    //  Fake TID
    rng.get_int32_next(0xffffffff, 0xffffffff);

    stats.pid = 0;
    for (size_t c = 0; c < rolls; c++){
#if 1
        stats.pid = rng.get_int32_next(0xffffffff, 0xffffffff);
#else
        stats.pid = rng.get_int32();
        rng.next();
#endif
    }

    for (int c = 0; c < max_ivs;){
        uint32_t index = rng.get_int32_next(6, 7);
        if (stats.ivs[index] == -1){
            stats.ivs[index] = 31;
            c++;
        }
    }
    for (int c = 0; c < 6; c++){
        if (stats.ivs[c] == -1){
            stats.ivs[c] = rng.get_int32() & 31;
            rng.next();
        }
    }

    return stats;
}

bool matches(const PokemonStats& filter, const PokemonStats& stats){
    if (filter.ec != stats.ec){
        return false;
    }
    if ((filter.pid ^ stats.pid) & 0xefffffff){
        return false;
    }
    for (int c = 0; c < 6; c++){
        if (filter.ivs[c] >= 0 && filter.ivs[c] != stats.ivs[c]){
            return false;
        }
    }

    return true;
}

void print(uint64_t seed, size_t rolls, int max_ivs){
    PokemonStats stats = generate(seed, rolls, max_ivs);

    std::lock_guard<std::mutex> lg(print_lock);
    std::cout << tostr_hex_padded(seed, 16) << " (rolls = " << rolls << "): ";
    std::cout << "EC = " << tostr_hex_padded(stats.ec, 8);
    std::cout << ", PID = " << tostr_hex_padded(stats.pid, 8);
    std::cout << ", IVs(" << max_ivs << ") = {"
              << stats.ivs[0] << ", "
              << stats.ivs[1] << ", "
              << stats.ivs[2] << ", "
              << stats.ivs[3] << ", "
              << stats.ivs[4] << ", "
              << stats.ivs[5] << "}";

    std::cout << std::endl;
}














}
