#include <windows.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <strsafe.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_BUF 256
using namespace std;

volatile char str[256] = { 0 };

int main()
{
	HANDLE hMutex[2];	// some handles
	HANDLE hMsg, hEnd;

	hMutex[0] = OpenMutex(SYNCHRONIZE, FALSE, "MutexP1");
	hMutex[1] = OpenMutex(SYNCHRONIZE, FALSE, "MutexP2");

	if (hMutex[0] == NULL || hMutex[1] == NULL)
		printf("Can't open Mutex\r\n");

	FILE* f;	// file for message
	char fName[16];	// filename

	printf("Waiting for ending other Writers...\r\n");
	while (true)	// wait while other writers end it's work
	{
		if (WaitForSingleObject(hMutex[0], 500) == WAIT_OBJECT_0)	// get MessageA
		{
			hMsg = OpenEvent(EVENT_MODIFY_STATE, FALSE, "MessageA");	// open this event
			strcpy(fName, "MessageA.txt");	// map filename
			break;
		}
		if (WaitForSingleObject(hMutex[1], 500) == WAIT_OBJECT_0)	// same
		{
			hMsg = OpenEvent(EVENT_MODIFY_STATE, FALSE, "MessageB");
			strcpy(fName, "MessageB.txt");
			break;
		}
	}

	hEnd = OpenEvent(EVENT_MODIFY_STATE, FALSE, "EndP");	// open end writer event
	if (hMsg == NULL || hEnd == NULL)
		printf("Can't open Event\r\n");

	int i = 0;
	TCHAR buf[MAX_BUF] = { 0 };	// buffer
	GetEnvironmentVariable("msgCount", buf, MAX_BUF);	// total msg
	LPTSTR endPtr;
	int msgCount = _tcstod(buf, &endPtr);	// to int

	while (i < msgCount)	// wait all msgs
	{
		//printf("Enter %d message(-s):\r\n", msgCount - i);	// show info
		//_tscanf("%s", buf, 255);	// get text in buffer

		f = fopen(fName, "wt");	// open file
		TCHAR buf[] = { '1', '2', '3', '4', '\0' };
		fwrite(buf, sizeof(TCHAR), _tcslen(buf), f);	// save txt
		fclose(f);	// close

		printf("Sending message...\r\n");
		SetEvent(hMsg);	// msg send
		Sleep(1000);	// wait a little for reader work
		i++;
	}

	SetEvent(hEnd);	// writer ends
	ReleaseMutex(hMutex[0]);	// close handles
	ReleaseMutex(hMutex[1]);
	CloseHandle(hMutex[0]);
	CloseHandle(hMutex[1]);
	CloseHandle(hMsg);
	CloseHandle(hEnd);

	return 0;
}