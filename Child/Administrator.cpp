#include <windows.h>
#include <iostream>
#include <strsafe.h>
#include <tchar.h>
#define MAX_BUF 256
using namespace std;

int msgCount, pCount;	// messages count, r/w count
TCHAR buf[MAX_BUF] = { 0 };
HANDLE hMutexP1, hMutexP2, hMutexP3, hMutexC1, hMutexC2, hMutexC3, hMsgOfFourDigits;	// mutex for maximum 2 current r/w
HANDLE hMsg, hEndP, hEndC;	// end session events

bool getRaW();
bool getMSG();

int main()
{
	if (!getRaW() || !getMSG())	// get r/w and messages
	{
		printf("Incorrect value!\r\n");
		return 0;
	}

	//printf("MSG: %d\r\nRaW: %d\r\n", msgCount, pCount);
	hMutexP1 = CreateMutex(NULL, FALSE, "MutexP1");
	hMutexP2 = CreateMutex(NULL, FALSE, "MutexP2");
	hMutexP3 = CreateMutex(NULL, FALSE, "MutexP3");
	hMutexC1 = CreateMutex(NULL, FALSE, "MutexC1");
	hMutexC2 = CreateMutex(NULL, FALSE, "MutexC2");
	hMutexC3 = CreateMutex(NULL, FALSE, "MutexC3");
	hMsgOfFourDigits = CreateEvent(NULL, TRUE, FALSE, "MsgOfFourDigits");
	hEndP = CreateEvent(NULL, TRUE, FALSE, "EndP");
	hEndC = CreateEvent(NULL, TRUE, FALSE, "EndC");

	StringCbPrintf(buf, MAX_BUF, "%d", msgCount);	// total msg per session
	SetEnvironmentVariable("msgCount", buf);

	int i = 0;

	while (i < pCount)	// run r/w count
	{
		//printf("%d", pCount);
		i++;
		STARTUPINFO si, si2;	// some vars
		PROCESS_INFORMATION pi, pi2;
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		ZeroMemory(&si2, sizeof(STARTUPINFO));
		si2.cb = sizeof(STARTUPINFO);

		char fName[20];
		hMsg = OpenEvent(EVENT_MODIFY_STATE, FALSE, "MsgOfFourDigits");	// open this event
		if (hMsg == NULL)
			printf("Can't open Event\r\n");
		strcpy(fName, "MsgOfFourDigits.txt");	// map filename
		FILE* f = fopen(fName, "wt");	// open file
		TCHAR buf[] = { '1', '2', '3', '4', '\0' };
		fwrite(buf, sizeof(TCHAR), _tcslen(buf), f);	// save txt
		fclose(f);	// close

		SetEvent(hMsg);	// msg send
		Sleep(1000);	// wait a little for reader work
		printf("Sending message...\r\n");

		// start writer and reader
		if (!CreateProcess("C:/Users/Roman-PC/Source/Repos/OSLab2/Debug/Parent.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))	 // relative path
		{
			printf("Can't start Parent.exe\r\n");
			return GetLastError();
		}

		CloseHandle(hMsg);	// close event, wait new msg

		hMsg = OpenEvent(EVENT_MODIFY_STATE, FALSE, "MsgOfFourDigits");	// open this event
		if (hMsg == NULL)
			printf("Can't open Event\r\n");
		strcpy(fName, "MsgOfFourDigits.txt");	// map filename
		f = fopen(fName, "wt");	// open file
		fwrite(buf, sizeof(TCHAR), _tcslen(buf), f);	// save txt
		fclose(f);	// close

		SetEvent(hMsg);	// msg send
		Sleep(1000);	// wait a little for reader work
		printf("Sending message...\r\n");

		if (!CreateProcess("C:/Users/Roman-PC/Source/Repos/OSLab2/Debug/Child.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si2, &pi2))
		{
			printf("Can't start Child.exe\r\n");
			return GetLastError();
		}
		CloseHandle(hMsg);
	}

	// open end session events
	int ends = 0;
	HANDLE hEnd[2];
	hEnd[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "EndP");
	hEnd[1] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "EndC");

	while (ends < pCount)	// wait all sessions end
	{
		WaitForMultipleObjects(2, hEnd, TRUE, INFINITE);
		ResetEvent(hEnd[0]);	// reset events for next session
		ResetEvent(hEnd[1]);
		ends++;
		printf("Child and Parent session closed\r\n");
	}

	printf("\r\nInput any char to exit...\r\n");
	scanf_s("%d", &pCount);

	// close all handles
	CloseHandle(hMutexP1);
	CloseHandle(hMutexP2);
	CloseHandle(hMutexP3);
	CloseHandle(hMutexC1);
	CloseHandle(hMutexC2);
	CloseHandle(hMutexC3);
	//CloseHandle(hMsg);
	CloseHandle(hMsgOfFourDigits);
	CloseHandle(hEndP);
	CloseHandle(hEndC);

	return 0;
}

bool getRaW()
{
	printf("Enter number of Parents/Children (max 10): ");

	if (scanf_s("%d", &pCount) == 0)
		return false;

	if (pCount > 10)
	{
		pCount = 10;
		printf("Number of Parents/Children  set to 10 (max)\r\n");
	}

	if (pCount < 1)
	{
		pCount = 1;
		printf("Number of Parents/Children  set to 1 (min)\r\n");
	}

	return true;
}

bool getMSG()
{
	printf("Enter number of messages (max 10): ");

	if (scanf_s("%d", &msgCount) == 0)
		return false;

	if (msgCount > 10)
	{
		msgCount = 10;
		printf("Number of messages set to 10 (max)\r\n");
	}

	if (msgCount < 1)
	{
		msgCount = 1;
		printf("Number of messages set to 1 (min)\r\n");
	}

	return true;
}
