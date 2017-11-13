#ifndef ___SoundKit_h
#define ___SoundKit_h

#include <windows.h>

class SoundKit {
public:
    static void Initialize(HWND hWnd);
    static void Cleanup();

    static void PlayBGM(char* filename);
    static void StopBGM();
    static void LoadSE(int no, char* filename);
    static void PlaySE(int no);
};

#endif // ___SoundKit_h


