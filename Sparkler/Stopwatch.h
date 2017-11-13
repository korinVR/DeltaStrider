#ifndef ___Stopwatch_h
#define ___Stopwatch_h

#include <windows.h>

#pragma comment(lib, "winmm.lib")

namespace Sparkler {

class Stopwatch {
public:
    Stopwatch()
    {
        LARGE_INTEGER n;
        
        if (QueryPerformanceFrequency(&n)) {
            usesPerformanceCounter = true;
            frequency = n.QuadPart;
        } else {
            usesPerformanceCounter = false;
            frequency = 1000;
        }
        timeBeginPeriod(1);
        Reset();
    }

    virtual ~Stopwatch()
    {
        timeEndPeriod(1);
    }
    
    void Reset()
    {
        accumulateCount = 0;
        running = false;
    }
    
    void Start()
    {
        if (!running) {
            startCount = GetCount();
            running = true;
        }
    }
    
    void Stop()
    {
        if (running) {
            accumulateCount += GetCount() - startCount;
            running = false;
        }
    }
    
    double GetTime()
    {
        LONGLONG count;

        if (running) {
            count = accumulateCount + GetCount() - startCount;
        } else {
            count = accumulateCount;
        }
        return (double) count / frequency;
    }
    
private:
    bool running;
    LONGLONG accumulateCount;
    LONGLONG startCount;
    LONGLONG frequency;
    bool usesPerformanceCounter;
    
    LONGLONG GetCount()
    {
        if (usesPerformanceCounter) {
            LARGE_INTEGER n;
            
            QueryPerformanceCounter(&n);
            return n.QuadPart;
        } else {
            return timeGetTime();
        }
    }
};

} // namespace Sparkler

#endif // ___Stopwatch_h


