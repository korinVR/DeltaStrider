#ifndef ___DetectMemoryLeaks_h
#define ___DetectMemoryLeaks_h

#ifdef WIN32
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
    #ifdef _DEBUG
        #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #endif
#else
#endif

#endif // ___DetectMemoryLeaks_h


