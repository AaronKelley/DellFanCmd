// SOURCE: https://github.com/424778940z/dell-fan-utility

#ifndef BZH_DELL_SMM_IO_DRV_FUN_H
#define BZH_DELL_SMM_IO_DRV_FUN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bzh_dell_smm_io_codes.h"
#include "bzh_dell_smm_io_drv_opt.h"

// Define the various device type values.  Note that values used by Microsoft
// Corporation are in the range 0-32767, and 32768-65535 are reserved for use
// by customers.

#define FILE_DEVICE_BZH_DELL_SMM 0x0000B424

// Macro definition for defining IOCTL and FSCTL function control codes.
// Note that function codes 0-2047 are reserved for Microsoft Corporation,
// and 2048-4095 are reserved for customers.

#define BZH_DELL_SMM_IOCTL_KEY 0xB42

// Define our own private IOCTL

#define IOCTL_BZH_DELL_SMM_RWREG     CTL_CODE(FILE_DEVICE_BZH_DELL_SMM,  \
    BZH_DELL_SMM_IOCTL_KEY,      \
    METHOD_BUFFERED,        \
    FILE_ANY_ACCESS)

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

typedef struct {
    unsigned long cmd;    // = cmd
    unsigned long data;   // = data
    unsigned long stat1;  // = 0
    unsigned long stat2;  // = 0
} SMBIOS_PKG;

extern SMBIOS_PKG smm_pkg;

void display_error(ULONG errorcode);
ULONG _stdcall dell_smm_io(ULONG cmd, ULONG data);

ULONG dell_smm_io_set_fan_lv(ULONG fan_no, ULONG lv);

#ifdef __cplusplus
}
#endif

#endif // BZH_DELL_SMM_IO_DRV_FUN_H

