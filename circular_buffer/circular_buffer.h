#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <memory>
#include <any>
#include <atomic>
#include <sstream>
#include <array>

class Circular_buffer
{
    static constexpr std::size_t m_max = 10;
    std::array<std::any, m_max> m_buffor;
    std::atomic<std::size_t> m_index = 0;
public:
    Circular_buffer();
    void put(std::any item);

    template<typename T2>
    std::array<T2, m_max> getDatabase() const{
        std::array<T2, m_max> data;
        for(std::size_t i = m_index,j =0; i < m_max; ++i, ++j){
            if (m_buffor.at(i).has_value())
            {
                data[j] =  std::any_cast<T2>(m_buffor.at(i)) ;
            }
        }
        return data;
    }

    template<typename T>
    std::string dump() const {
        std::stringstream ss;
        for(std::size_t i = 0; i < m_max; ++i){
            if (m_buffor.at(i).has_value())
            {
                ss << i << ": " << std::any_cast<T>(m_buffor.at(i)) << std::endl;
            }
        }
        return ss.str();
    }

};

#endif // CIRCULAR_BUFFER_H
