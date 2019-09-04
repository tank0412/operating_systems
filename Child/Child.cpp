#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <strsafe.h>
#define MAX_BUF 256
using namespace std;

int main()
{
	HANDLE hMutex[2];
	HANDLE hMsg, hEnd;

	hMutex[0] = OpenMutex(SYNCHRONIZE, FALSE, "MutexC1");
	hMutex[1] = OpenMutex(SYNCHRONIZE, FALSE, "MutexC2");

	if (hMutex[0] == NULL || hMutex[1] == NULL)
		printf("Can't open Mutex\r\n");

	FILE* f;	// file w/ message
	char fName[16];	// filename

	printf("Waiting for ending other Readers...\r\n");
	while (true)	// while all mutex' is busy
	{
		if (WaitForSingleObject(hMutex[0], 500) == WAIT_OBJECT_0)	// MutexA gotcha
		{
			hMsg = OpenEvent(EVENT_ALL_ACCESS, FALSE, "MessageA");	// open MessageA event
			strcpy(fName, "MessageA.txt");	// file for message is MessageA.txt
			break;
		}
		if (WaitForSingleObject(hMutex[1], 500) == WAIT_OBJECT_0)	// same fro B
		{
			hMsg = OpenEvent(EVENT_ALL_ACCESS, FALSE, "MessageB");
			strcpy(fName, "MessageB.txt");
			break;
		}
	}

	hEnd = OpenEvent(EVENT_MODIFY_STATE, FALSE, "EndC");	// open end reader event
	if (hMsg == NULL || hEnd == NULL)
		printf("Can't open Event\r\n");

	int i = 0;
	TCHAR buf[MAX_BUF] = { 0 };	// buffer
	GetEnvironmentVariable("msgCount", buf, MAX_BUF);	// total msg
	LPTSTR endPtr;
	int msgCount = _tcstod(buf, &endPtr);	// to int

	while (i < msgCount)	// wait all msgs
	{
		printf("Waiting message...\r\n");

		DWORD dwRes = WaitForSingleObject(hMsg, INFINITE);
		if (dwRes != WAIT_OBJECT_0)	// wait failed
			printf("Wait for single object failed\r\nERROR: %d ERRORCODE: %d\r\n", dwRes, GetLastError());

		f = fopen(fName, "rt");	// open msg file
		memset(buf, 0, sizeof(buf));	// null previous buffer
		fread(buf, sizeof(TCHAR), MAX_BUF, f);	// read msg from file
		fclose(f);	// close file

		_tprintf("Message recieved: %s\r\n", buf);	// print msg

		ResetEvent(hMsg);	// reset event, wait new msg
		i++;
	}

	SetEvent(hEnd);	// end reader process
	printf("\r\nClose in 5 seconds...\r\n");
	Sleep(5000);
	// close handles
	ReleaseMutex(hMutex[0]);
	ReleaseMutex(hMutex[1]);
	CloseHandle(hMutex[0]);
	CloseHandle(hMutex[1]);
	CloseHandle(hMsg);
	CloseHandle(hEnd);

	return 0;
}