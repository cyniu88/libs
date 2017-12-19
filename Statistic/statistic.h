#ifndef STATISTIC_H
#define STATISTIC_H
#include <iostream>
#include <deque>
#include <numeric>
#include <algorithm>

template <class T>
class STATISTIC
{
public:
    STATISTIC(unsigned int size): m_size(size)
    {

    }
    void resize(unsigned int i){
        if (i < m_size){
            while(m_dequeue.size()> i){
                pop_back();
            }
        }
        m_size = i;
    }
    T size(){
        return  static_cast<T>(m_dequeue.size());
    }
    void push_front(T v){
        if (m_dequeue.size() >= m_size){
            pop_back();
        }
        m_dequeue.push_front(v);
    }
    void push_back(T v){
        if (m_dequeue.size() >= m_size){
            pop_front();
        }
        m_dequeue.push_back(v);
    }
    void pop_back(){
        m_dequeue.pop_back();
       // std::cout <<"pop_back"<<std::endl;
    }
    void pop_front(){
        m_dequeue.pop_front();
       // std::cout <<"pop_front"<<std::endl;
    }
    ///////////////////////////////////////////// statistic /////////////////////////////
    T median(){
        auto backup = m_dequeue;
        std::sort(backup.begin(), backup.end());
        if (backup.size() % 2 != 0){
            return backup[backup.size() / 2];
        }
        else{
            T m = backup[backup.size() / 2] + backup[(backup.size() / 2)-1];
            return m /2 ;
        }
    }

    T average(){
        T av = std::accumulate(m_dequeue.begin(), m_dequeue.end(), 0) / size();
        return av;
    }

    T max(){
        T max = m_dequeue[0];
        for (auto v : m_dequeue){
            if (v > max){
                max = v;
            }
        }
        return max;
    }

    T min(){
        T min = m_dequeue[0];
        for (auto v : m_dequeue){
            if (v < min){
                min = v;
            }
        }
        return min;
    }

    float trend(){
        int down = 0;
        int eq = 0;
        int up = 0;
        T first = m_dequeue[0];

        for (auto i = 1 ; i < m_dequeue.size(); ++i){
            if (first < m_dequeue[i]){ up++;}
            if (first == m_dequeue[i]){ eq++;}
            if (first > m_dequeue[i]){ down++;}
            first = m_dequeue[i];
        }
        std::cout <<"up "<<up<<" eq "<< eq << " down "<< down << std::endl;
        return 2.2;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    void print(){
        for(auto n : m_dequeue){
            std::cout << ","<< n ;
        }
        std::cout << " " <<std::endl;
    }

private:
    unsigned int m_size;
    std::deque <T> m_dequeue;
};

#endif // STATISTIC_H
