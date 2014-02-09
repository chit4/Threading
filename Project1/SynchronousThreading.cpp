#include "MasterDependencies.h"
#include "SynchronousThreadingClass.h"

int main()
{
	Point* p = new Point();
	ThreadY* thread1 = new ThreadY(true, p);

	HANDLE handleToThread1;
	unsigned uiThread1ID;

	handleToThread1 = (HANDLE)_beginthreadex(NULL, 
											  0,
											  ThreadY::ThreadStaticStartUp,
											  thread1,
											  CREATE_SUSPENDED,
											  &uiThread1ID);
	if (handleToThread1 == 0)
	{
		cout << "Failed to create thread1\n";
	}

	DWORD dwExitCode;

	GetExitCodeThread(handleToThread1, &dwExitCode);
	cout << "Initial thread1 exit code......" << dwExitCode << "\n";

	ThreadY* thread2 = new ThreadY(false, p);

	HANDLE handleToThread2;
	unsigned uiThread2ID;

	handleToThread2 = (HANDLE)_beginthreadex(NULL,
											 0,
											 ThreadY::ThreadStaticStartUp,
											 thread2,
											 CREATE_SUSPENDED,
											 &uiThread2ID);

	if (handleToThread2 == 0)
	{
		cout << "Failed to create thread2\n";
	}

	GetExitCodeThread(handleToThread2, &dwExitCode);
	cout << "Initial thread2 exit code....." << dwExitCode << "\n";

	ResumeThread(handleToThread1);
	ResumeThread(handleToThread2);

	Sleep(100);

	WaitForSingleObject(handleToThread1, INFINITE);
	WaitForSingleObject(handleToThread2, INFINITE);

	GetExitCodeThread(handleToThread1, &dwExitCode);
	cout << "thread1 exited with code ......." << dwExitCode << "\n";

	GetExitCodeThread(handleToThread2, &dwExitCode);
	cout << "thread2 exited with code ......." << dwExitCode << "\n";

	delete thread1;
	thread1 = NULL;

	delete thread2;
	thread2 = NULL;

	cout << "Main Thread exiting....";

	getchar();
}