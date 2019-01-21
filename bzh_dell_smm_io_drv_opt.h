// SOURCE: https://github.com/424778940z/dell-fan-utility

#ifndef BZH_DELL_SMM_IO_DRV_OPT_H
#define BZH_DELL_SMM_IO_DRV_OPT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <windows.h>
#include <winioctl.h>
#include <stdbool.h>

extern HANDLE hDriver;
extern bool IsInitialized;
extern wchar_t szDriverPath[32768];

bool _stdcall BDSID_Initialize();
void _stdcall BDSID_Shutdown();
bool _stdcall BDSID_InstallDriver(PWSTR pszDriverPath, bool IsDemandLoaded = false);
bool _stdcall BDSID_RemoveDriver();

bool _stdcall BDSID_StartDriver();
bool _stdcall BDSID_StopDriver();

#ifdef __cplusplus
}
#endif

#endif // BZH_DELL_SMM_IO_DRV_OPT_H

