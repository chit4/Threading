#include "MasterDependencies.h"

//#define WITH_SYNCHRONIZATION

#pragma region Point Declartion. Thread2 modifies it and Thread1 reads from it. 
class Point
{
public:
	Point();
	virtual ~Point();

	void Move(int xor, int yor)
	{
#ifdef WITH_SYNCHRONIZATION
		EnterCriticalSection(&m_CriticalSection);
#endif
		x = xor;
		y = yor; 
	

#ifdef WITH_SYNCHRONIZATION
	LeaveCriticalSection(&m_CriticalSection);
#endif
	}

	char* ToString()
	{
		int xCopy;
		int yCopy;
#ifdef WITH_SYNCHRONIZATION
		EnterCriticalSection(&m_CriticalSection);
#endif
		
		xCopy = x;
		yCopy = y;

#ifdef WITH_SYNCHRONIZATION
		LeaveCriticalSection(&m_CriticalSection);
#endif

		static char szTemp[128];

		if (xCopy == yCopy)
		{
			sprintf(szTemp, "(%d, %d) \n", xCopy, yCopy);
		}
		else
		{
			sprintf(szTemp, "(%d, %d) ERROR\n", xCopy, yCopy);
		}

		return szTemp;
	}

private:
	int x;
	int y;

#ifdef WITH_SYNCHRONIZATION
	CRITICAL_SECTION m_CriticalSection;
#endif
};

Point::Point()
{
#ifdef WITH_SYNCHRONIZATION
	InitializeCriticalSection(&m_CriticalSection);
#endif
}

Point::~Point()
{
#ifdef WITH_SYNCHRONIZATION
	DeleteCriticalSection(&m_CriticalSection);
#endif
}
#pragma endregion


class ThreadY
{
public:
	ThreadY(bool isMover, Point* p);
	~ThreadY();

	static unsigned _stdcall ThreadStaticStartUp(void *pThis)
	{
		ThreadY* pThreadY = (ThreadY*)pThis;
		pThreadY->StartUp();

		return 1;
	}

	void StartUp()
	{
		if (mover)
		{
			for (int i = 1; i <= 200000000; ++i)
			{
				point->Move(i, i);
			}
		}
		else
		{
			for (int i = 0; i < 200; i++)
			{
				cout << point->ToString();
				Sleep(1);
			}
		}
	}


private:
	Point* point;
	bool mover;
};

ThreadY::ThreadY(bool isMover, Point* p)
{
	mover = isMover;
	point = p;
}

ThreadY::~ThreadY()
{
}