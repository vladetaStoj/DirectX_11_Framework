#ifndef _AUXILLARY_H_
#define _AUXILLARY_H_

#include <windows.h>

class Auxillary
{
public:

	Auxillary();
	~Auxillary();

	//Timer functions
	void StartTimer();
	double GetTime();
	double GetFrameTime();

private:

	double countsPerSecond;
	__int64 CounterStart;

	__int64 frameTimeOld;
};

#endif