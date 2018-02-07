#ifndef STATISTIC_H
#define STATISTIC_H
#include <iostream>
#include <deque>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <vector>

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
                pop_front();
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
    T sum(){
        return std::accumulate(m_dequeue.begin(), m_dequeue.end(), static_cast<T>(0));
    }

    T average(){
        T av  = sum() /size();
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

    T range(){
        return max() - min();
    }

    T standardDeviation(){
        double standardDeviation = 0.0;
        T _av = average();

        for(int i = 0; i < size(); ++i){
            standardDeviation += pow(m_dequeue.at(i) - _av, 2);
        }
        return sqrt(standardDeviation / size());
    }

    T coefficientOfVariation(){

        return (standardDeviation()/average()) /** 100*/;
    }

    std::vector<T> mode(){
        //        int i = 0;
        std::vector<T> _mode;
        //        auto backup = m_dequeue;
        //        std::sort(backup.begin(), backup.end());
        //        for(b : backup){

        auto backup = m_dequeue;
        std::sort(backup.begin(), backup.end());
        T number = backup[0];
        T mode;
        for (unsigned int i = 1, countMode = 1, count = 1; i < backup.size(); ++i) {
            if (backup[i] == number)
                ++countMode;
            if (countMode > count) {
                count = countMode;
                mode = number;
            }
            number = backup[i];
        }
        _mode.push_back(mode);
        return _mode;
    }

    float trend(){
        int down = 0;
        int eq = 0;
        int up = 0;
        int lp = 0;
        T diff = 0;
        T first = m_dequeue[0];

        for (auto i = 1 ; i < m_dequeue.size(); ++i){
            if (first < m_dequeue[i]){
                up++;
                if (m_dequeue[i] - first > diff){
                    diff = m_dequeue[i] - first;
                    lp = i;
                }
            }
            if (first == m_dequeue[i]){ eq++;}
            if (first > m_dequeue[i]){
                if (diff < first - m_dequeue[i] ){
                    diff = first - m_dequeue[i];
                    lp = i;
                }
                down++;
            }
            first = m_dequeue[i];
        }
        std::cout <<"up "<<up<<" eq "<< eq << " down "<< down <<" max diff "<< diff<<" lp "<<lp << std::endl;
        return 2.2;
    }

    bool isMoreDiff(T diff){
        if (m_dequeue.size()>2){
            T d = m_dequeue.at( m_dequeue.size()-2)
                    - m_dequeue.at( m_dequeue.size() - 1);
            d = fabs(d);
            if (d > diff && m_alarm == false){
                m_alarm = true;
                return true;
            }
            if (d <= diff){
                m_alarm = false;
                return false;
            }
        }
        else {
            return false;
        }
        return false;
    }
    std::pair<double,double> getLast2(){
        if (m_dequeue.size()>2){
            return std::make_pair(static_cast<double>(m_dequeue.at( m_dequeue.size()-2)),
                                  static_cast<double>(m_dequeue.at( m_dequeue.size()-1))    );
        }
        return std::make_pair(0.0,0.0);
    }

    /////////////////////////////////////////////////////////////////////////////////////
    void print(){
        for(auto n : m_dequeue){
            std::cout << ","<< n ;
        }
        std::cout << " " <<std::endl;
    }


    std::string stats(){

        std::stringstream ss(" brak danych =(");
        if(size()>0)
        {
            ss.str("");
            ss <<"rozmiar tablicy: "<< size() <<std::endl
              << "min: "<< min() <<std::endl
              << "max: "<< max()<<std::endl
              << "srednia " << average() <<std::endl
              << "mediana " << median()  <<std::endl
              << "odchylenie st "<< standardDeviation() << std::endl
              << "wspolczynnik zmiennosci " << coefficientOfVariation() <<"%"<< std::endl
              << "Dominanta " ;
            auto mo = mode();
            for ( auto n : mo){
                ss << n << "| ";
            }
            ss << std::endl
               << "data " <<  std::endl;
            for(auto n : m_dequeue){
                ss << "|"<< n ;
            }
        }
        ss <<  std::endl;
        return ss.str();
    }

private:
    unsigned int m_size;
    std::deque <T> m_dequeue;
    bool m_alarm = false;
};

#endif // STATISTIC_H
