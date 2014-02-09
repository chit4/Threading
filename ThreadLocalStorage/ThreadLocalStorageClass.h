#include "MasterDependencies.h"

#define WITH_SYNCHRONIZATION

class ThreadX
{
public:
	ThreadX();
	virtual ~ThreadX();

	char* threadName;

	void TMain(void)
	{
#ifdef WITH_SYNCHRONIZATION
		EnterCriticalSection(&m_CriticalSection);
#endif

		for (int i = 1; i <= 5; i++)
		{
			++message1;
			++message2;
			++message3;
		}

		cout << "Thread: " << threadName << " message1: " << message1 << " message2 :" << message2 << " message3:" << message3 << "\n";

#ifdef WITH_SYNCHRONIZATION
		LeaveCriticalSection(&m_CriticalSection);
#endif
	}

	static unsigned _stdcall ThreadStaticTMain(void* pThis)
	{
		ThreadX* pThreadX = (ThreadX*)pThis;
		pThreadX->TMain();

		return 1;
	}

private:
	int message1;
	static int message2;
	static _declspec(thread) int message3;

#ifdef WITH_SYNCHRONIZATION
	CRITICAL_SECTION m_CriticalSection;
#endif
};

int ThreadX::message2 = 20;
int ThreadX::message3 = 30;

ThreadX::ThreadX()
{
	message1 = 10;
#ifdef WITH_SYNCHRONIZATION
	InitializeCriticalSection(&m_CriticalSection);
#endif
}

ThreadX::~ThreadX()
{
#ifdef WITH_SYNCHRONIZATION
	DeleteCriticalSection(&m_CriticalSection);
#endif
}