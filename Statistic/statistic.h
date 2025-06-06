#pragma once

#include <iostream>
#include <deque>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <vector>


template <class T>
struct TREND_DATA{
    int down = 0;
    double downPercent = 0;
    int up = 0;
    double upPercent = 0;
    int eq = 0;
    double eqPercent = 0;
    T down_sum = 0;
    T up_sum = 0;
    void clear(){
        down = 0;
        up = 0;
        eq = 0;
        down_sum = 0;
        up_sum = 0;
    }
    void countPercent(){
        int sum = eq + up + down;
        if(sum == 0)
            return;
        eqPercent = eq * 100.0f / sum;
        upPercent = up * 100.0f / sum;
        downPercent = down * 100.0f / sum;
    }
    std::string getStringInfo(){
        countPercent();
        std::stringstream ret;
        //ret.precision(2);
        ret << "eq: " << eqPercent << "% | "
            << "up: " << upPercent << "% | "
            << "down: " << downPercent << "%"
            << std::endl
            << "eq: " << eq
            << " | up: " << up
            << " | down: " << down
            << " | down_sume: " << down_sum
            << " | up_sum: " << up_sum
            << std::endl;

        return ret.str();
    }
};

template <class T>
class STATISTIC
{
public:
    explicit STATISTIC(std::size_t size = 100): m_size(size)
    {

    }
    void resize(std::size_t i){
        if (i < m_size){
            while(m_dequeue.size()> i){
                pop_front();
            }
        }
        m_size = i;
    }
    T getSize(){
        return  static_cast<T>((m_dequeue.size() == 0)?1:m_dequeue.size());
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
    void keepLatestOnly(){
        if (!m_dequeue.empty()) {
            T latest = m_dequeue.back();
            m_dequeue.clear();
            m_dequeue.push_back(latest);
        }
    }
    void erase(){
        m_dequeue.clear();
    }
    ///////////////////////////////////////////// statistic /////////////////////////////
    T median(){
        auto backup = m_dequeue;
        std::sort(backup.begin(), backup.end());
        if(backup.empty())
            return static_cast<T>(0);
        if(backup.size() % 2 != 0){
            return backup[backup.size() / 2];
        }
        else{
            T m = backup[backup.size() / 2] + backup[(backup.size() / 2)-1];
            return m /2;
        }
    }
    T sum(){
        return std::accumulate(m_dequeue.begin(), m_dequeue.end(), static_cast<T>(0));
    }

    T average(){
        T av  = sum() /getSize();
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

    double standardDeviation(){
        double standardDeviation = 0.0;
        T _av = average();

        for(int i = 0; i < getSize(); ++i){
            standardDeviation += pow(m_dequeue.at(i) - _av, 2);
        }
        return sqrt(standardDeviation / getSize());
    }

    double coefficientOfVariation(){

        return (standardDeviation()/average()) /** 100*/;
    }

    T mode(){

        T _mode = 0;
        T _modeTemp = 0;
        int counter = 1;
        int modeCounter = 1;
        auto backup = m_dequeue;
        if(m_dequeue.empty()){
            return static_cast<T>(0);
        }
        if(m_dequeue.size() == 1)
        {
            return m_dequeue.at(0);
        }
        std::sort(backup.begin(), backup.end());
#ifdef BT_TEST
        std::cout << " " << std::endl;
        for (auto i : backup)
        {
            std::cout << i << " ";
        }
        std::cout << " " << std::endl;
#endif
        _mode = _modeTemp = backup.at(0);
        backup.pop_front();
        for (auto b : backup)
        {
            if (_modeTemp == b)
            {
                modeCounter++;
            }
            else
            {
                _modeTemp = b;
                modeCounter = 1;
            }

            if(counter < modeCounter)
            {
                counter = modeCounter;
                _mode = _modeTemp;
            }
        }
#ifdef BT_TEST
        std::cout << " moda: " << _mode << " wystepuje razy " << counter << std::endl;
#endif
        return _mode;
    }

    TREND_DATA<T> trend(){
        m_trendData.clear();
        if(m_dequeue.size() == 0)
            return m_trendData;

        T first = m_dequeue[0];
        for (auto i = 1; i < m_dequeue.size(); ++i){
            if(first > m_dequeue.at(i)){
                ++m_trendData.down;
                m_trendData.down_sum += (first - m_dequeue.at(i));
            }
            else if(first < m_dequeue.at(i)){
                ++m_trendData.up;
                m_trendData.up_sum += (m_dequeue.at(i) - first);
            }
            else
                ++m_trendData.eq;
            first = m_dequeue.at(i);
        }

        return m_trendData;
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
        return false;
    }
    std::pair<double,double> getLast2(){
        if (m_dequeue.size()>2){
            return std::make_pair(static_cast<double>(m_dequeue.at( m_dequeue.size()-2)),
                                  static_cast<double>(m_dequeue.at( m_dequeue.size()-1))    );
        }
#ifdef BT_TEST
        puts("no data - return 0.0 0.0");
#endif
        return std::make_pair(0.0,0.0);
    }

    /////////////////////////////////////////////////////////////////////////////////////
    void print(){
        for(auto n : m_dequeue){
            std::cout << ","<< n;
        }
        std::cout << " " <<std::endl;
    }


    std::string stats(){

        std::stringstream ss(" brak danych =(");
        if(m_dequeue.size() > 0)
        {
            ss.str("");
            ss <<"rozmiar tablicy: "<< getSize() <<std::endl
               << "min: "<< min() <<std::endl
               << "max: "<< max()<<std::endl
               << "srednia " << average() <<std::endl
               << "mediana " << median()  <<std::endl
               << "odchylenie st "<< standardDeviation() << std::endl
               << "wspolczynnik zmiennosci " << coefficientOfVariation() <<"%"<< std::endl
               << "Dominanta " << mode() << std::endl
               << "trend " << trend().getStringInfo();


            ss << std::endl
               << "data " <<  std::endl;
            for(auto n : m_dequeue){
                ss << "|"<< n;
            }
        }
        ss <<  std::endl;
        return ss.str();
    }

private:
    unsigned int m_size;
    std::deque <T> m_dequeue;
    bool m_alarm = false;
    TREND_DATA <T> m_trendData;
};

