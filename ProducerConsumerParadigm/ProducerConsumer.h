#include "MasterDependencies.h"

//#define WITH_SYNCHRONIZATION
#define MSG_BUF_SIZE 128

#pragma region MessageBufferClass which acts as a platform for both the Producer and Consumer classes
class MessageBuffer
{
public:
	MessageBuffer();
	~MessageBuffer();

	void SetMessage(char* s)
	{
		cout << "In MessageBuffer::SetMessage()\n";
#ifdef WITH_SYNCHRONIZATION
		DWORD dwWaitResult = WaitForSingleObject(m_hMutex, INFINITE);

		if (dwWaitResult != WAIT_OBJECT_0)
		{
			cout << "WaitForSingleObject() failed in MessageBuffer::SetMessage()\n";
			return;
		}

		cout << "SetMessage() acquired mutex\n";
#endif

		if(strlen(s) >= MSG_BUF_SIZE)
			s[MSG_BUF_SIZE - 1] = '\0';

		char* pch = &messageText[0];

		while (*s)
		{
			*pch++ = *s++;
			Sleep(5);
		}

		*pch = '\0';

#ifdef WITH_SYNCHRONIZATION
		cout << "SetMessage() pulsing Event\n";

		if (!SetEvent(m_hEvent))
		{
			cout << "SetEvent() failed in SetMessage()\n";
		}

		cout << "SetMessage() relesing mutex \n";
		if (! ReleaseMutex(m_hMutex))
		{
			cout << "ReleaseMutex() failed in MessageBuffer::SetMessage()\n";
		}
#endif
	}

	void ProcessMessages()
	{
		while (m_bContinueProcessing)
		{
#ifdef WITH_SYNCHRONIZATION
			DWORD dwWaitResult = WaitForSingleObject(m_hEvent, 2000);

			if((dwWaitResult == WAIT_TIMEOUT && m_bContinueProcessing == false))
			{
				break;
			}
			else if (dwWaitResult == WAIT_ABANDONED)
			{
				cout << "WaitForSingleObject(1) failed in MessageBuffer::ProcessMessages()\n ";
				return;
			}
			else if (dwWaitResult == WAIT_OBJECT_0)
			{
				cout << "ProcessMessages() saw Event\n";
			}

			dwWaitResult = WaitForSingleObject(m_hMutex, INFINITE);

			if (dwWaitResult != WAIT_OBJECT_0)
			{
				cout << "WaitForSingleObject(2) failed in MessageBuffer::ProcessMessages()\n";
				return;
			}
			cout << "ProcessMessages() acquired Mutex\n";
#endif
			if (strlen(messageText) != 0)
			{
				cout << "Processed new message ....... " << messageText;

				messageText[0] = '\0';
			}

#ifdef WITH_SYNCHRONIZATION
			cout << "\nProcessMessages() releasing mutex\n\n";
			if(! ReleaseMutex(m_hMutex))
			{
				cout << "ReleaseMutex() failed in MessageBuffer::ProcessMessages()\n";
			}
#endif
		} // end-of-loop m_bContinueProcessing
		
	}

	void DieDieDie(void)
	{
		m_bContinueProcessing = false;
	}
		
private:
	char messageText[MSG_BUF_SIZE];
	bool m_bContinueProcessing;

#ifdef WITH_SYNCHRONIZATION
	HANDLE m_hMutex;
	HANDLE m_hEvent;
#endif
};

MessageBuffer::MessageBuffer()
{
	memset(messageText, '\0', sizeof(messageText));
	m_bContinueProcessing = true;

#ifdef WITH_SYNCHRONIZATION
	cout << "Creating mutex in MessageBuffer constructor\n";
	m_hMutex = CreateMutex(NULL,
							false,
							"MessageBufferMutex"
							);
	if (m_hMutex == NULL)
	{
		cout << "CreateMutex() failed in MessageBuffer constructor\n";
	}

	//Create Auto-Reset event
	m_hEvent = CreateEvent(NULL,
						   FALSE,
						   FALSE,
						   "MessageBufferEvent"
						   );
#endif
}

MessageBuffer::~MessageBuffer()
{
#ifdef WITH_SYNCHRONIZATION
	CloseHandle(m_hMutex);
	CloseHandle(m_hEvent);
#endif
}
#pragma endregion

#pragma region CreateMessages class which acts as Producer to produce stuff and then it commits suicide
class CreateMessages
{
public:
	CreateMessages(MessageBuffer* m);
	~CreateMessages();

	void CreateMessageEntryPoint()
	{
		char szTemp[MSG_BUF_SIZE];

		for (int i =1; i < 5; ++i)
		{
			sprintf(szTemp, "%d%d%d%d%d%d%d%d%d%d", i, i, i, i, i, i, i, i, i, i); 
			//szTemp[0] = (char)i; szTemp[1] = (char)i; szTemp[2] = (char)i; szTemp[3] = (char)i; szTemp[4] = i; szTemp[5] = i; szTemp[6] = i;
			//szTemp[7] = i; szTemp[8] = i; szTemp[9] = i; 

			//cout << szTemp;

			message->SetMessage(szTemp);
			cout << "\n";

			Sleep(1000);
		}

		cout << "CreateMessages thread terminating....\n";
	}

	static unsigned _stdcall CreateMessageEntryPoint(void* pThis)
	{
		CreateMessages* pointerToCreateMessages = (CreateMessages*)pThis;
		pointerToCreateMessages->CreateMessageEntryPoint();

		_endthreadex(2);

		return 2;
	}

private:
	MessageBuffer* message;

};


CreateMessages::CreateMessages(MessageBuffer* m)
{
	message = m;
}

CreateMessages::~CreateMessages()
{
}
#pragma endregion

class ProcessMessages
{
public:
	ProcessMessages(MessageBuffer* m);
	~ProcessMessages();

	void ProcessMessageEntryPoint()
	{
		message->ProcessMessages();
	}

	static unsigned _stdcall ProcessMessagesStaticEntryPoint(void* pThis)
	{
		ProcessMessages* pointerToProcessMessages = (ProcessMessages*)pThis;
		pointerToProcessMessages->ProcessMessageEntryPoint();

		cout << "ProcessMessages thread terminating\n";

		return 1;
	}

	void DieDieDie(void)
	{
		message->DieDieDie();
	}

private:
	MessageBuffer* message;
};

ProcessMessages::ProcessMessages(MessageBuffer* m)
{
	message = m;
}

ProcessMessages::~ProcessMessages()
{
}