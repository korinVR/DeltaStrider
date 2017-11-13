#ifndef ___Log_H
#define ___Log_H

#include <cstdio>
#include <cstdarg>

using namespace std;

namespace Sparkler {

class Log {
private:
    Log() {}

public:
    static void Open(const char* filename);
    static void Close();
    static void Printf(const char* format, ...);
};

} // namespace Sparkler

#endif // ___Log_H


