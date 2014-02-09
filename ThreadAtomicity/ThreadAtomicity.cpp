#include "ThreadAtomicityClass.h";

int main()
{

	HANDLE handleToThread1;
	unsigned uiThread1ID;

	handleToThread1 = (HANDLE)_beginthreadex(NULL,
											 0,
											 TMain,
											 NULL,
											 CREATE_SUSPENDED,
											 &uiThread1ID);

	if (handleToThread1 == 0)
	{
		cout << "Failed to create thread1\n";
	}

	DWORD dwExitCode;
	
	GetExitCodeThread(handleToThread1, &dwExitCode);
	cout << "Initial thread1 exit code ..... " << dwExitCode << "\n";

	HANDLE handleToThread2;
	unsigned uiThreadID2;

	handleToThread2 = (HANDLE)_beginthreadex(NULL,
											 0,
											 TMain,
											 NULL,
											 CREATE_SUSPENDED,
											 &uiThreadID2);

	if (handleToThread2 == 0)
	{
		cout << "Failed to create thread2 .....";
	}

	GetExitCodeThread(handleToThread2, &dwExitCode);

}