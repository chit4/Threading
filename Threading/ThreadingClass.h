#include "MasterHeaderDependencies.h"

class ThreadX
{
	public:
		char* threadName;

		//Constructor
		ThreadX();
		ThreadX(int startValue, int endValue, int frequency);

		//Destructor
		~ThreadX();

		static unsigned _stdcall ThreadStaticEntryPoint(void* pThis)
		{
			ThreadX *pthX = (ThreadX*)pThis;
			pthX->TrueThreadEntryPoint();

			return 1;
		}

		void TrueThreadEntryPoint()
		{
			for (int i = loopStart; i < loopEnd; i++)
			{
				if (i % dispFrequency == 0)
				{
					cout << threadName << " : i = " << i << "\n";
				}
			}

			cout << threadName << " thread terminating\n";
		}

	private:
		int loopStart;
		int loopEnd;
		int dispFrequency;

};

ThreadX::ThreadX(int startValue, int endValue, int frequency)
{
	loopStart = startValue;
	loopEnd = endValue;
	dispFrequency = frequency;
}

ThreadX::~ThreadX()
{
}

class Point
{
public:
	Point();
	~Point();

private:
	int x;
	int y;
};

Point::Point()
{

}

Point::~Point()
{
}