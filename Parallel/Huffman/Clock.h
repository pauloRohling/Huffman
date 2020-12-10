#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <time.h>
#include <Windows.h>

using namespace std;

class Clock {
public:
    Clock() {
        srand(time(nullptr));
    }

    void Tick() {
        this->start = clock();
    }

    double Tock() {
        return (double(clock() - start) / (CLOCKS_PER_SEC));
    }

    void ZaWarudo() {
        Sleep(2000);
    }

private:
    clock_t start;
};

#endif // CLOCK_H
