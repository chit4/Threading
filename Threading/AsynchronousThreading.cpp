#include "MasterHeaderDependencies.h"
#include "ThreadingClass.h"

void MySillyProcess(void*);

int main()
{
	cout << "In main()....\n";

	//Creating ThreadX object
	ThreadX* thread1 = new ThreadX(0, 1000000, 20000);

	HANDLE handleToThread1;
	unsigned uiThread1ID;

	//The initial state of the thread is "SUSPEND"
	handleToThread1 = (HANDLE) _beginthreadex(NULL,
											   0,
											   ThreadX::ThreadStaticEntryPoint,
											   thread1,
											   CREATE_SUSPENDED,
											   &uiThread1ID);

	if (handleToThread1 == 0)
	{
		cout << "Failed to create thread.....\n";
	}

	DWORD dwExitCode;
	
	GetExitCodeThread(handleToThread1, &dwExitCode);
	cout << "Initial thread1 exit code ........" << dwExitCode << "\n";

	thread1->threadName = "thread1";

	ThreadX* thread2 = new ThreadX(-1000000, 0, 20000);

	HANDLE handleToThread2;
	unsigned uiThread2ID;

	handleToThread2 = (HANDLE) _beginthreadex(NULL,
											   0,
											   ThreadX::ThreadStaticEntryPoint,
											   thread2,
											   CREATE_SUSPENDED,
											   &uiThread2ID);

	if (handleToThread2 == 0)
	{
		cout << "Failed to create thread......\n";
	}

	GetExitCodeThread(handleToThread2, &dwExitCode);
	cout << "Initial thread2 exit code ......." << dwExitCode << "\n";

	thread2->threadName = "thread2";

	//Threads which were initially suspended are "RESUMED" here
	ResumeThread(handleToThread1);
	ResumeThread(handleToThread2);

	WaitForSingleObject(handleToThread1, INFINITE);
	WaitForSingleObject(handleToThread2, INFINITE);

	GetExitCodeThread(handleToThread1, &dwExitCode);
	cout << "thread1 exited with code " << dwExitCode << "\n";

	GetExitCodeThread(handleToThread2, &dwExitCode);
	cout << "thread2 exited with code " << dwExitCode << "\n";

	CloseHandle(handleToThread1);
	CloseHandle(handleToThread2);

	delete thread1;
	thread1 = NULL;

	delete thread2;
	thread2 = NULL;

	cout << "\n\nPrimary thread terminating......\n";
	/*_beginthread(MySillyProcess, 0, (void*) 12);

	MySillyProcess((void*)-5);
	Sleep(1000);*/

	getchar();
}

void MySillyProcess(void* args)
{
	cout << "\nIn sub process() " << (INT_PTR)args;
}