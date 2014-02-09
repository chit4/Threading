#include "ProducerConsumer.h"

int main()
{
	DWORD dwExitCode;

	MessageBuffer* messageBuffer = new MessageBuffer();

	ProcessMessages* processMessages = new ProcessMessages(messageBuffer);

	HANDLE handleToConsumer;
	unsigned uiConsumerThreadID;

	handleToConsumer = (HANDLE)_beginthreadex(NULL,
											  0,
											  ProcessMessages::ProcessMessagesStaticEntryPoint,
											  processMessages,
											  CREATE_SUSPENDED,
											  &uiConsumerThreadID);
	if (handleToConsumer == 0)
	{
		cout << "Failed to create consumer thread\n";
	}

	GetExitCodeThread(handleToConsumer, &dwExitCode);
	cout << "Initial consumer thread exit code ........ " << dwExitCode << "\n";

	ResumeThread(handleToConsumer);

	CreateMessages* createMessages = new CreateMessages(messageBuffer);

	HANDLE handleToProducer;
	unsigned uiProducerThreadID;

	handleToProducer = (HANDLE) _beginthreadex(NULL,
											   0,
											   CreateMessages::CreateMessageEntryPoint,
											   createMessages,
											   CREATE_SUSPENDED,
											   &uiProducerThreadID);
	
	if (handleToProducer == 0)
	{
		cout << "Failed to create producer thread\n";
	}

	ResumeThread(handleToProducer);

	WaitForSingleObject(handleToProducer, INFINITE);

	GetExitCodeThread(handleToProducer, &dwExitCode);
	cout << "Producer thread exited with code ...... " << dwExitCode << "\n";

	processMessages->DieDieDie();

	WaitForSingleObject(handleToConsumer, INFINITE);

	GetExitCodeThread(handleToConsumer, &dwExitCode);
	cout << "Consumer thread exited with code ......" << dwExitCode << "\n";

	CloseHandle(handleToProducer);
	CloseHandle(handleToConsumer);

	delete createMessages;
	createMessages = NULL;

	delete processMessages;
	processMessages = NULL;

	delete messageBuffer;
	messageBuffer = NULL;

	cout << "Primary thread terminating \n";

	getchar();
}