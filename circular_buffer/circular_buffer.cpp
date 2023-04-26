#include "circular_buffer.h"

Circular_buffer::Circular_buffer()
{

}

void Circular_buffer::put(std::any item)
{
    m_buffor[m_index] = item;
    ++m_index;
    if(m_index == m_max)
        m_index = 0;
}
/*
template<typename T> std::string Circular_buffer::dump() const
{
    std::stringstream ss;
    for(std::size_t i = 0; i < m_max; ++i){
        if (m_buffor.at(i).has_value())
        {
            ss << i << ": " << std::any_cast<T>(m_buffor.at(i)) << std::endl;
        }
    }
    return ss.str();
}
*/
