#include "mainHeader.h"

/*
 * LoadDriver - Loads Driver
 *
 * @hService: pointer to a SC_HANDLE for opening a service
 *
 * @hSCManager: pointer to a SC_HANDLE to estabilish a connection to the SC manager
 *
 * Return:
 * 	- TRUE the driver was loaded
 * 	- FALSE failed to load the driver
 */

BOOLEAN LoadDriver(_Out_ SC_HANDLE* hService, _Out_ SC_HANDLE* hSCManager, _In_ wchar_t* DriverName) {
	BOOLEAN status = FALSE;
	/*
	 * Get handle to servive manager with desired acesss
	 */

	*hSCManager = OpenSCManager(NULL, NULL, GENERIC_WRITE);

	if (*hSCManager != NULL) {
		*hService = OpenService(*hSCManager, DriverName, GENERIC_EXECUTE | DELETE);
		if (*hService != NULL)
			if (StartService(*hService, 0, NULL))
				status = TRUE;
			else
				printf("Failed to start service %d", GetLastError());
		else
			printf("Failed to get service handle %d", GetLastError());
	}
	else {
		printf("Failed to get service manager handle %d", GetLastError());
		hService = NULL;
	}

	return status;
}

/*
 * UnloadDriver - Unloads Driver
 *
 * @hService: pointer to a SC_HANDLE for stoping the driver and be closed
 *
 * @hSCManager: pointer to a SC_HANDLE to be closed
 *
 */
void UnloadDriver(_In_ SC_HANDLE* hService, _In_ SC_HANDLE* hSCManager) {
	SERVICE_STATUS status;

	/*
	 * Sends control code to the driver to stop it
	 */

	ControlService(*hService, SERVICE_CONTROL_STOP, &status);

	CloseServiceHandle(*hService);
	CloseServiceHandle(*hSCManager);
}


/**
 * GetDriverHandle - Open a handle to a driver.
 * @DriverName: Name of the driver to open a handle to.
 *
 * This function opens a handle to a driver with the specified name.
 *
 * Returns: A handle to the driver if successful, otherwise an error code.
 */
HANDLE GetDriverHandle(wchar_t* DriverName) {
	HANDLE handle;
	wchar_t driverPath[MAX_PATH];

	// Concatenate the received driver name with "\\.\"
	swprintf(driverPath, MAX_PATH, L"\\\\.\\%s", DriverName);

	// Open a handle to the driver file
	handle = CreateFile(driverPath, GENERIC_WRITE | GENERIC_READ,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (handle == INVALID_HANDLE_VALUE) {
		printf("Failed to open driver: %d\n", GetLastError());
	}

	return handle;
}

