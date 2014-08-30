#pragma  once

#include "Globals.h"

class cTimer
{
public:
	cTimer();
	~cTimer();

	void  Initialize();
	void  UpdateFPS();
	void  UpdateTimer();
	int   GetFps();
	float GetTime();
private:
	int m_Fps, m_Count;
	unsigned long m_StartTimeFPS;
 
	INT64 m_CountsPerSec;
	INT64 m_PrevTime;
	float m_SecPerCount;
	INT64 m_StartTimeTimer;
	float m_DeltaTime;
};


