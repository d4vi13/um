#include "mainHeader.h"


#define CAPTURE_TIME 10000
#define FS_DRIVER_NAME L"MiniFilter"

BOOLEAN captureOn = TRUE;

VOID CALLBACK StopCapture() {
	captureOn = FALSE;
	return;
}



int main() {
	SC_HANDLE hService, hSCManager;
	HANDLE timerHandle;
	INFO Info;
	wchar_t FsDriverName[] = FS_DRIVER_NAME;

	LoadDriver(&hService, &hSCManager,FsDriverName);

	//Get Driver Hanlde


	//cria temporizador
	CreateTimerQueueTimer(&timerHandle, NULL, (WAITORTIMERCALLBACK)StopCapture, NULL, CAPTURE_TIME, 0, WT_EXECUTEDEFAULT);

	//loop que retira da lista
	while (captureOn) {
		DeviceIoControl();
	}

	//change third parameter to a function that 
	(void)DeleteTimerQueueTimer(NULL, &timerHandle, NULL);

	UnloadDriver(&hService, &hSCManager);


}