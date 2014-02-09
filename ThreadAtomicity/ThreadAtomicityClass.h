#include "MasterDependencies.h"

static bool interlocked = false;

const int maxCount = 10000000;
static LONG value = 0;

unsigned _stdcall TMain(void* args)
{
	if (interlocked)
	{
		for (int i = 1; i < maxCount; i++)
		{
			InterlockedIncrement(&value);
		}
	}
	else
	{
		for (int i = 1; i < maxCount; i++)
		{
			++value;
		}
	}

	return 3;
}