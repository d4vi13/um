#include "mainHeader.h"


/*
 * LogMjFunc - Log major function information
 * @MjFunc: MJFUNC struct containing major function details
 * @logFile: FILE pointer to the log file
 *
 * Writes the name of the major function to the log file.
 */
void LogMjFunc(MJFUNC MjFunc, FILE* logFile) {
    fprintf(logFile, "\"mjFunc\": ");
    fprintf(logFile, "\"%lu\",\n", MjFunc.Code);

    return;
}

/*
 * LogTime - Log time information
 * @TimeFields: TIME_FIELDS struct containing time details
 * @logFile: FILE pointer to the log file
 *
 * Writes time information to the log file in the format: "Hour:Minute:Second:Milliseconds".
 */
void LogTime(TIME_FIELDS TimeFields, FILE* logFile) {
    fprintf(logFile, "\"date\": ");
    fprintf(logFile, "\"%d/%d/%d\",\n", TimeFields.Day, TimeFields.Month, TimeFields.Year);

    fprintf(logFile, "\"time\": ");
    fprintf(logFile, "\"%d:%d:%d\",\n", TimeFields.Hour, TimeFields.Second, TimeFields.Milliseconds);
    return;
}

/*
 * LogProcInfo - Log process information
 * @ProcInfo: PROC_INFO struct containing process details
 * @logFile: FILE pointer to the log file
 *
 * Writes process information to the log file including PID, SID, and elevation status.
 */
void LogProcInfo(PROC_INFO ProcInfo, FILE* logFile) {
    fprintf(logFile, "\"pid\": ");
    fprintf(logFile, "\"%lu\",\n", ProcInfo.PID);
    
    fprintf(logFile, "\"sid\": ");
    fprintf(logFile, "\"%lu\",\n", ProcInfo.SID);
    
    fprintf(logFile, "\"isElevated\": ");
    fprintf(logFile, "\"%ld\",\n", ProcInfo.IsElevated);

    return;
}

/*
 * LogBaseInfo - Log base information
 * @Info: INFO struct containing base information
 * @logFile: FILE pointer to the log file
 *
 * Logs the base information including major function, time, and process information to the log file.
 */
void LogBaseInfo(INFO Info, FILE* logFile) {

    LogTime(Info.TimeFields, logFile);

    LogMjFunc(Info.MjFunc, logFile);

    LogProcInfo(Info.ProcInfo, logFile);
}

/*
 * LogFileInfo - Log file information
 * @FileInfo: Pointer to PFILE_INFO struct containing file details
 * @logFile: FILE pointer to the log file
 *
 * Writes file path and file name to the log file.
 */
void LogFileInfo(PFILE_INFO FileInfo, FILE* logFile) {
    fprintf(logFile, "\"path\": ");
    fprintf(logFile, "\"%ws\",\n", FileInfo->Path);

    fprintf(logFile, "\"fileName\": ");
    fprintf(logFile, "\"%ws\" \n", FileInfo->FinalName);

    return;
}

/*
 * LogInfo - Log information based on IOCTL code
 * @Info: INFO struct containing information to be logged
 * @logFile: FILE pointer to the log file
 * @IOCTL_CODE: DWORD representing the IOCTL code
 *
 * Logs information based on the IOCTL code. Currently, only supports logging file information.
 */
void LogInfo(INFO Info, FILE* logFile, DWORD IOCTL_CODE) {
    fprintf(logFile,"{\n");
    LogBaseInfo(Info, logFile);

    switch (IOCTL_CODE) {
    case IOCTL_FILE_LOG:
        LogFileInfo(&Info.Info.FileInfo, logFile);
        break;
    default:
        printf("Invalid IOCTL code given\n");
        break;
    }
    fprintf(logFile, "}\n");

    fflush(logFile); // Flush the buffer after each write
}
