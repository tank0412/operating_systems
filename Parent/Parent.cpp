#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <strsafe.h>
#define MAX_BUF 256
using namespace std;

int main()
{
	HANDLE hMutex[3], hMutex2[3];
	HANDLE hMsg, hEnd;

	hMutex[0] = OpenMutex(SYNCHRONIZE, FALSE, "MutexP1");
	hMutex[1] = OpenMutex(SYNCHRONIZE, FALSE, "MutexP2");
	hMutex[2] = OpenMutex(SYNCHRONIZE, FALSE, "MutexP3");

	hMutex2[0] = OpenMutex(SYNCHRONIZE, FALSE, "MutexC1");
	hMutex2[1] = OpenMutex(SYNCHRONIZE, FALSE, "MutexC2");
	hMutex2[2] = OpenMutex(SYNCHRONIZE, FALSE, "MutexC3");

	if (hMutex[0] == NULL || hMutex[1] == NULL || hMutex[2] == NULL || hMutex2[0] == NULL || hMutex2[1] == NULL || hMutex2[2] == NULL)
		printf("Can't open Mutex\r\n");

	FILE* f;	// file w/ message
	char fName[20];	// filename

	printf("Waiting for ending other Children...\r\n");
	while (true)	// while all mutex' is busy
	{
		if (WaitForSingleObject(hMutex[0], 500) == WAIT_OBJECT_0 && WaitForSingleObject(hMutex2[0], 500) == WAIT_OBJECT_0)	// MutexA gotcha
		{
			hMsg = OpenEvent(EVENT_ALL_ACCESS, FALSE, "MsgOfFourDigits");	// open MessageA event
			strcpy(fName, "MsgOfFourDigits.txt");	// file for message is MessageA.txt
			break;
		}
	}

	hEnd = OpenEvent(EVENT_MODIFY_STATE, FALSE, "EndP");	// open end reader event
	if (hMsg == NULL || hEnd == NULL)
		printf("Can't open Event\r\n");

	int i = 0;
	TCHAR buf[MAX_BUF] = { 0 };	// buffer
	GetEnvironmentVariable("msgCount", buf, MAX_BUF);	// total msg
	LPTSTR endPtr;
	int msgCount = _tcstod(buf, &endPtr);	// to int


	printf("Waiting message...\r\n");

	DWORD dwRes = WaitForSingleObject(hMsg, INFINITE);
	if (dwRes != WAIT_OBJECT_0)	// wait failed
		printf("Wait for single object failed\r\nERROR: %d ERRORCODE: %d\r\n", dwRes, GetLastError());

	f = fopen(fName, "rt");	// open msg file
	memset(buf, 0, sizeof(buf));	// null previous buffer
	fread(buf, sizeof(TCHAR), MAX_BUF, f);	// read msg from file
	fclose(f);	// close file

	_tprintf("Message recieved: %s\r\n", buf);	// print msg

	//ResetEvent(hMsg);	// reset event, wait new msg

	SetEvent(hEnd);	// end reader process
	printf("\r\nClose in 5 seconds...\r\n");
	Sleep(5000);
	// close handles
	ReleaseMutex(hMutex[0]);
	ReleaseMutex(hMutex[1]);
	ReleaseMutex(hMutex[2]);
	ReleaseMutex(hMutex2[0]);
	ReleaseMutex(hMutex2[1]);
	ReleaseMutex(hMutex2[2]);
	CloseHandle(hMutex[0]);
	CloseHandle(hMutex[1]);
	CloseHandle(hMutex[2]);
	CloseHandle(hMutex2[0]);
	CloseHandle(hMutex2[1]);
	CloseHandle(hMutex2[2]);
	CloseHandle(hMsg);
	CloseHandle(hEnd);

	return 0;
}