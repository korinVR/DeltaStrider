#include <windows.h>

#include <exception>
#include <stdexcept>
using namespace std;

#include <boost/smart_ptr.hpp>
using namespace boost;

#include "sslib2/dsound/DirectSound.h"
#include "sslib2/dsound/OggFileReader.h"
#include "sslib2/dsound/PlainWavFileReader.h"
#include "sslib2/dsound/StaticSoundBuffer.h"
#include "sslib2/dsound/StreamSoundBuffer.h"
#include "sslib2/dsound/WarFileReader.h"
using namespace sslib2;

#include "SoundKit.h"

namespace {
	CDirectSound directSound;
	CStreamSoundBuffer bgmBuffer;

	const int SE_MAX = 256;
	CStaticSoundBuffer seBuffer[SE_MAX];

	bool playing;
}

void SoundKit::Initialize(HWND hWnd)
{
	if (!directSound.Create(hWnd)) {
		throw runtime_error("");
	}
}

void SoundKit::Cleanup()
{
}

void SoundKit::PlayBGM(char* filename)
{
	char buf[256];
	sprintf(buf, "sound/%s", filename);

	shared_ptr<COggFileReader> reader(new COggFileReader());
	if (!reader->Open(buf)) {
		throw runtime_error("");
	}
	if (!bgmBuffer.Create(directSound, reader)) {
		throw runtime_error("");
	}
	bgmBuffer.Play();
	playing = true;
}

void SoundKit::StopBGM()
{
	if (playing) {
		playing = false;
		bgmBuffer.Stop();
	}
}

void SoundKit::LoadSE(int no, char* filename)
{
	char buf[256];
	sprintf(buf, "sound/%s", filename);

	shared_ptr<CPlainWavFileReader> reader(new CPlainWavFileReader());
	if (!reader->Open(buf)) {
		throw runtime_error("");
	}
	if (!seBuffer[no].Create(directSound, reader)) {
		throw runtime_error("");
	}
}

void SoundKit::PlaySE(int no)
{
	seBuffer[no].Play();
}

