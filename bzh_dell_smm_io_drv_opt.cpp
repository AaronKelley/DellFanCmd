// SOURCE: https://github.com/424778940z/dell-fan-utility

#ifdef __cplusplus
extern "C"
{
#endif

#include "bzh_dell_smm_io_drv_opt.h"

HANDLE hDriver = INVALID_HANDLE_VALUE;
bool IsInitialized = false;
wchar_t szDriverPath[32768];
bool g_Is64BitOS;

//***********************************************************************************************
bool _stdcall BDSID_InstallDriver(PWSTR pszDriverPath, bool IsDemandLoaded)
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;

    // Remove any previous instance of the driver

    BDSID_RemoveDriver();

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCManager)
    {
        // Install the driver

        hService = CreateService(hSCManager,
            L"BZHDELLSMMIO",
            L"BZHDELLSMMIO",
            SERVICE_ALL_ACCESS,
            SERVICE_KERNEL_DRIVER,
            (IsDemandLoaded == true) ? SERVICE_DEMAND_START : SERVICE_SYSTEM_START,
            SERVICE_ERROR_NORMAL,
            pszDriverPath,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL);

        CloseServiceHandle(hSCManager);

        if (hService == NULL)
            return false;
    }
    else
        return false;

    CloseServiceHandle(hService);

    return true;
}


bool _stdcall BDSID_RemoveDriver()
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    LPQUERY_SERVICE_CONFIG pServiceConfig;
    DWORD dwBytesNeeded;
    DWORD cbBufSize;
    bool bResult;

    BDSID_StopDriver();

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (!hSCManager)
    {
        return false;
    }

    hService = OpenService(hSCManager, L"BZHDELLSMMIO", SERVICE_ALL_ACCESS);
    CloseServiceHandle(hSCManager);

    if (!hService)
    {
        return false;
    }

    bResult = QueryServiceConfig(hService, NULL, 0, &dwBytesNeeded);

    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        cbBufSize = dwBytesNeeded;
        pServiceConfig = (LPQUERY_SERVICE_CONFIG)malloc(cbBufSize);
        bResult = QueryServiceConfig(hService, pServiceConfig, cbBufSize, &dwBytesNeeded);

        if (!bResult)
        {
            free(pServiceConfig);
            CloseServiceHandle(hService);
            return bResult;
        }

        // If service is set to load automatically, don't delete it!
        if (pServiceConfig->dwStartType == SERVICE_DEMAND_START)
        {
            bResult = DeleteService(hService);
        }
    }

    CloseServiceHandle(hService);

    return bResult;
}


bool _stdcall BDSID_StartDriver()
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    bool bResult;

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCManager)
    {
        hService = OpenService(hSCManager, L"BZHDELLSMMIO", SERVICE_ALL_ACCESS);

        CloseServiceHandle(hSCManager);

        if (hService)
        {
            bResult = StartService(hService, 0, NULL) || GetLastError() == ERROR_SERVICE_ALREADY_RUNNING;

            CloseServiceHandle(hService);
        }
        else
            return false;
    }
    else
        return false;

    return bResult;
}


bool _stdcall BDSID_StopDriver()
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    SERVICE_STATUS ServiceStatus;
    bool bResult;

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCManager)
    {
        hService = OpenService(hSCManager, L"BZHDELLSMMIO", SERVICE_ALL_ACCESS);

        CloseServiceHandle(hSCManager);

        if (hService)
        {
            bResult = ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus);

            CloseServiceHandle(hService);
        }
        else
            return false;
    }
    else
        return false;

    return bResult;
}

//***********************************************************************************************


typedef UINT (WINAPI* GETSYSTEMWOW64DIRECTORY)(LPTSTR, UINT);

bool BDSID_GetDriverPath()
{
    PWSTR pszSlash;
    if (!GetModuleFileName(GetModuleHandle(NULL), szDriverPath, sizeof(szDriverPath)))
        return false;
    pszSlash = wcsrchr(szDriverPath, '\\');
    if (pszSlash)
        pszSlash[1] = 0;
    else
        return false;

    wcscat_s(szDriverPath, L"bzh_dell_smm_io_x64.sys");

    return true;
}

bool __stdcall BDSID_Initialize()
{
    bool bResult;
    hDriver = CreateFile(L"\\\\.\\BZHDELLSMMIO",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    // If the driver is not running, install it
    if (hDriver == INVALID_HANDLE_VALUE)
    {
        BDSID_GetDriverPath();
        bResult = BDSID_InstallDriver(szDriverPath, true);
        if (!bResult)
            return false;
        bResult = BDSID_StartDriver();
        if (!bResult)
            return false;
        hDriver = CreateFile(L"\\\\.\\BZHDELLSMMIO",
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (hDriver == INVALID_HANDLE_VALUE)
            return false;
    }
    IsInitialized = true;
    return true;
}

void _stdcall BDSID_Shutdown()
{
    if (hDriver != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hDriver);
    }
    BDSID_RemoveDriver();
    IsInitialized = false;
}

#ifdef __cplusplus
}
#endif
