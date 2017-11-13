#include "Log.h"

#ifdef _WIN32
	#include <windows.h>
#endif
#include <cassert>
#include <exception>
#include <stdexcept>

using namespace std;

namespace Sparkler {

namespace {
	FILE* fp = NULL;
	
	const int BUF_SIZE = 512;
	char buf[BUF_SIZE];
}

void Log::Open(const char* filename)
{
	fp = fopen(filename, "w");
	if (fp == NULL) {
//		throw runtime_error("");
	}
}

void Log::Close()
{
	if (fp) {
		fclose(fp);
		fp = NULL;
	}
}

void Log::Printf(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	if (fp) {
		fputs(buf, fp);
		fflush(fp);
	}
#ifdef _WIN32
	OutputDebugString(buf);
#endif
}

} // namespace Sparkler


