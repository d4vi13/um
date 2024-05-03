#include "mainHeader.h"

BOOLEAN captureOn = TRUE;

int main() {
	SC_HANDLE hService, hSCManager;
	HANDLE timerHandle, DriverHandle;
	INFO Info;
	wchar_t DriverName[] = DRIVER_NAME;

	LoadDriver(&hService, &hSCManager,DriverName);

	//Get Driver Hanlde
    DriverHandle = GetDriverHandle(DrievrName);

	//cria temporizador
	CreateTimerQueueTimer(&timerHandle, NULL, (WAITORTIMERCALLBACK)StopCapture
                        , NULL, CAPTURE_TIME, 0, WT_EXECUTEDEFAULT);

	//loop que retira da lista
	while (captureOn) {
		DeviceIoControl(DriverHandle,IOCTL_FILE_LOG,
                        UM_MSG,sizeof(UM_MSG),&Info,
                        sizeof(Info),&BytesReturned,
                        NULL);
        if (BytesReturned > 0)
            printf("Hello baack from kernel mode\n");
	}

	//change third parameter to a function that 
	(void)DeleteTimerQueueTimer(NULL, &timerHandle, NULL);

	UnloadDriver(&hService, &hSCManager);

    CloseHandle(DriverHandle);

    return 0;
}

