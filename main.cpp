#include "mainHeader.h"

// Global variable to control capture state

static BOOLEAN captureOn = TRUE;

VOID CALLBACK StopCapture() {
    captureOn = FALSE;
    return;
}

int main(int argc, char **argv) {
    // Handle declarations
    SC_HANDLE hService, hSCManager;
    HANDLE timerHandle, DriverHandle;
    BOOLEAN loadDriver = TRUE;
    
    // Structure for information
    INFO Info;
    int devR;

    // Driver name
    wchar_t DriverName[] = DRIVER_NAME;

    DWORD BytesReturned;
    CHAR um_msg[] = "aloaloaloalo\n";

    // File pointer for logging
    FILE* logFile;
    errno_t err = fopen_s(&logFile, "Log.txt", "w");
    if (err != 0 || logFile == NULL) {
        printf("Error opening file for writing.\n");
        return 1; // Exit the program with an error code
    }

    // Load the driver
    if(loadDriver)
        LoadDriver(&hService, &hSCManager, DriverName);
    
    // Get a handle to the driver
    DriverHandle = GetDriverHandle(DriverName);

    // Create a timer to stop capture
    CreateTimerQueueTimer(&timerHandle, NULL, (WAITORTIMERCALLBACK)StopCapture,
        NULL, CAPTURE_TIME, 0, WT_EXECUTEDEFAULT);

    // Main loop for capturing
    while (captureOn) {
        // Send IOCTL to driver to capture information
        
        devR = DeviceIoControl(DriverHandle, IOCTL_FILE_LOG,
            &um_msg, sizeof(um_msg), &Info,
            sizeof(Info), &BytesReturned,
            NULL);

        if (!devR)
            printf("erro:%d\n", GetLastError());

        // Log captured information if any
        if (BytesReturned > 0) {
            LogInfo(Info, logFile, IOCTL_FILE_LOG);
        }
    }

    // Delete the timer
    (void)DeleteTimerQueueTimer(NULL, &timerHandle, NULL);

    // Unload the driver
    if(loadDriver)
        UnloadDriver(&hService, &hSCManager);

    // Close the driver handle
    CloseHandle(DriverHandle);

    return 0;
}
