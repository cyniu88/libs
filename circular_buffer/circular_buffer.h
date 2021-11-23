#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <memory>
#include <any>
#include <atomic>

class Circular_buffer
{
    static constexpr std::size_t m_max = 10;
    std::array<std::any, m_max> m_buffor;
    std::atomic<std::size_t> m_index = 0;
public:
    Circular_buffer();
    void put(std::any item);
    std::string dump() const ;
};

#endif // CIRCULAR_BUFFER_H
