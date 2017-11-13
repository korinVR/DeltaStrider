#ifndef ___InputKit_h
#define ___InputKit_h

class InputKit {
public:
	static void Initialize();
	static void Cleanup();
	static void Reset();
	static void Update();

	static void LoadSequence(char* filename);
	static void SaveSequence(char* filename);
	static void Record();
	static void Play();

	static bool Playing();
	static bool IsSequenceEnd();
	
public:
	static float stickX;
	static float stickY;

	static int stickUpCount;
	static int stickDownCount;
	static int stickLeftCount;
	static int stickRightCount;

	static bool fireButtonPressed;
	static bool statsButtonPressed;
	static bool viewButtonPressed;

	static int fireButtonCount;
	static int statsButtonCount;
	static int viewButtonCount;

	static bool aborted;
};

#endif // ___InputKit_h


