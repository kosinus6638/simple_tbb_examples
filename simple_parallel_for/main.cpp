#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"


static const std::size_t N = 200000000;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<double> Duration;


class ChangeElements {
    std::vector<int> *v;
    std::vector<int> *w;

public:
    void operator()( const tbb::blocked_range<std::size_t> &r ) const { 
        for( std::size_t i=r.begin(); i!=r.end(); ++i ) {
            if      ((*v)[i] > 0) (*w)[i] = 1;
            else if ((*v)[i] < 0) (*w)[i] = -1;
            else                  (*w)[i] = 0;
        }
    }

    ChangeElements(std::vector<int> *_v, std::vector<int> *_w) : v(_v), w(_w) {}
};


void ChangeElementsSequential(const std::vector<int> *v, std::vector<int> *w) {
    for(std::size_t i=0; i<(*v).size(); ++i) {
        if      ((*v)[i] > 0) (*w)[i] = 1;
        else if ((*v)[i] < 0) (*w)[i] = -1;
        else                  (*w)[i] = 0;
    }
}


int mrand() {
    return (rand() - RAND_MAX/2) % 3;
}


int main(void) {
    std::vector<int> v(N,0);
    std::vector<int> w(N,0);

    /* Time */
    auto start = Time::now();
    auto end = Time::now();
    Duration elapsed;

    /* Seed RNG */
    srand(time(0));

    /* Fill vector with random numbers */
    std::generate(v.begin(), v.end(), mrand);
    std::cout << "Vector filled" << std::endl;

    /* Serial task */
    start = Time::now();
    ChangeElementsSequential(&v,&w);
    end = Time::now();
    elapsed = end-start;

    std::cout << "Sequential time: " << elapsed.count() << "s\n";

    /* Parallel task */
    start = Time::now();
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,N), ChangeElements(&v, &w) );
    end = Time::now();
    elapsed = end-start;

    std::cout << "Parallel time: " << elapsed.count() << "s\n";

    return 0;
}
