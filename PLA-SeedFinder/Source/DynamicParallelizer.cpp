/*  Dynamic Parallelizer
 *
 *  From: https://github.com/PokemonAutomation/
 *
 */

#include "DynamicParallelizer.h"

namespace PokemonAutomation{


DynamicParallelizer::DynamicParallelizer(
    std::function<void(uint64_t start, uint64_t length)>&& function,
    uint64_t block_alignment,
    uint64_t total, size_t threads
)
    : m_function(std::move(function))
    , m_thread_count(threads == 0 ? std::thread::hardware_concurrency() : threads)
    , m_total(total)
    , m_threads(m_thread_count)
    , m_next(0)
    , m_completed(0)
{
    uint64_t blocks = m_thread_count * 4;
    m_block_size = (total + blocks - 1) / blocks;
    m_block_size = (m_block_size + block_alignment - 1) / block_alignment * block_alignment;
    for (std::thread& thread : m_threads){
        thread = std::thread(&DynamicParallelizer::thread_loop, this);
    }
}
DynamicParallelizer::~DynamicParallelizer(){
    for (std::thread& thread : m_threads){
        thread.join();
    }
}
void DynamicParallelizer::thread_loop(){
    uint64_t start = 0;
    uint64_t block = 0;
    while (true){
        if (block != 0){
            m_function(start, block);
        }
        std::unique_lock<std::mutex> lg(m_lock);
        if (block != 0){
            m_completed += block;
        }
        if (m_completed == m_total){
            return;
        }
        start = m_next;
        block = std::min(m_block_size, m_total - start);
        m_next = start + block;
    }
}






}
