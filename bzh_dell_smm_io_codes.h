// SOURCE: https://github.com/424778940z/dell-fan-utility

#ifndef BZH_DELL_SMM_IO_CODES
#define BZH_DELL_SMM_IO_CODES

/* This is some information about the codes for different functions on the Dell SMBIOS */

#define DELL_SMM_IO_DISABLE_FAN_CTL1    0x30a3
#define DELL_SMM_IO_ENABLE_FAN_CTL1     0x31a3
#define DELL_SMM_IO_DISABLE_FAN_CTL2    0x34a3
#define DELL_SMM_IO_ENABLE_FAN_CTL2     0x35a3

#define DELL_SMM_IO_SET_FAN_LV          0x01a3

#define DELL_SMM_IO_FAN_LV0             0x0000
#define DELL_SMM_IO_FAN_LV1             0x0001
#define DELL_SMM_IO_FAN_LV2             0x0002

#define DELL_SMM_IO_FAN1                0
#define DELL_SMM_IO_FAN2                1

#define DELL_SMM_IO_NO_ARG      0x0

#endif // BZH_DELL_SMM_IO_CODES
