#include "bzh_dell_smm_io_codes.h"
#include "bzh_dell_smm_io_drv_fun.h"

#include <iostream>
using namespace std;

// Function prototypes.
void usage();
void loadDriver();
void unloadDriver();

// Main program begins here.
int main(int argc, char* argv[])
{
    cout << "DellFanCmd 1.0.0\n";
    cout << "By Aaron Kelley\n";
    cout << "Licensed under GPLv3\n";
    cout << "Source code available at https://github.com/AaronKelley/DellFanCmd\n\n";
    cout << "Dell SMM IO driver by 424778940z\n";
    cout << "https://github.com/424778940z/bzh-windrv-dell-smm-io\n\n";
    cout << "Derived from \"Dell fan utility\" by 424778940z\n";
    cout << "https://github.com/424778940z/dell-fan-utility\n\n";

    if (argc != 2)
    {
        usage();
    }
    else
    {
        // Behavior variables.
        bool disableEcFanControl = false;
        bool enableEcFanControl = false;
        bool setFansTo100 = false;
        bool useAlternateCommand = false;

        // Figure out what was requested.
        if (strcmp(argv[1], "ec-disable") == 0)
        {
            disableEcFanControl = true;
            setFansTo100 = true;
        }
        else if (strcmp(argv[1], "ec-disable-nofanchg") == 0)
        {
            disableEcFanControl = true;
        }
        else if (strcmp(argv[1], "ec-enable") == 0)
        {
            enableEcFanControl = true;
        }
        else if (strcmp(argv[1], "ec-disable-alt") == 0)
        {
            disableEcFanControl = true;
            setFansTo100 = true;
            useAlternateCommand = true;
        }
        else if (strcmp(argv[1], "ec-disable-alt-nofanchg") == 0)
        {
            disableEcFanControl = true;
            useAlternateCommand = true;
        }
        else if (strcmp(argv[1], "ec-enable-alt") == 0)
        {
            enableEcFanControl = true;
            useAlternateCommand = true;
        }
        else
        {
            usage();
            exit(3);
        }

        // Execute request.

        // Load driver first.
        loadDriver();

        if (disableEcFanControl)
        {
            cout << "Attempting to disable EC control of the fan...\n";

            ULONG command = !useAlternateCommand ? DELL_SMM_IO_DISABLE_FAN_CTL1 : DELL_SMM_IO_DISABLE_FAN_CTL2;
            int result = dell_smm_io(command, DELL_SMM_IO_NO_ARG);

            if (result == -1)
            {
                cerr << "Failed.\n";
                unloadDriver();
                exit(1);
            }

            cout << " ...Success.\n";

            if (setFansTo100)
            {
                cout << "Setting fan 1 speed to maximum...\n";
                result = dell_smm_io_set_fan_lv(DELL_SMM_IO_FAN1, DELL_SMM_IO_FAN_LV2);
                if (result == -1)
                {
                    cerr << "Failed.\n";
                }

                cout << "Setting fan 2 speed to maximum...\n";
                result = dell_smm_io_set_fan_lv(DELL_SMM_IO_FAN2, DELL_SMM_IO_FAN_LV2);
                if (result == -1)
                {
                    cerr << "Failed.  (Maybe your system just has one fan?)\n";
                }
            }
            else
            {
                cout << "WARNING: CPU and GPU are not designed to run under load without active cooling.\n";
                cout << "Make sure that you have alternate fan speed control measures in place.\n";
            }
        }
        else if (enableEcFanControl)
        {
            cout << "Attempting to enable EC control of the fan...\n";

            ULONG command = !useAlternateCommand ? DELL_SMM_IO_ENABLE_FAN_CTL1 : DELL_SMM_IO_ENABLE_FAN_CTL2;
            int result = dell_smm_io(command, DELL_SMM_IO_NO_ARG);
            if (result == -1)
            {
                cerr << "Failed.\n";
                unloadDriver();
                exit(1);
            }

            cout << " ...Success.\n";
        }

        // Unload driver.
        unloadDriver();
    }

    return 0;
}

// Print out basic usage information.
void usage()
{
    cout << "Usage: DellFanCmd [command]\n";
    cout << "Available commands:\n";
    cout << "  ec-disable              Turn EC fan control off (fan goes to manual control)\n";
    cout << "  ec-disable-nofanchg     Turn EC fan control off and don't change the fan speed\n";
    cout << "  ec-enable               Turn EC fan control on (fan goes to automatic control)\n";
    cout << "  ec-disable-alt          Turn EC fan control off, alternate method\n";
    cout << "  ec-disable-alt-nofanchg Turn EC fan control off and don't change the fan speed, alternate method\n";
    cout << "  ec-enable-alt           Turn EC fan control on, alternate method\n";
}

// Load the SMM IO driver.
void loadDriver()
{
    cout << "Loading SMM IO driver...\n";
    bool result = BDSID_Initialize();
    if (!result)
    {
        cerr << "Failed.\n";
        cerr << "\n";
        cerr << "Please make sure that bzh_dell_smm_io_x64.sys is present in the working directory,\n";
        cerr << "and that measures needed to allow the signature to be verified have been performed.\n";
        cerr << "\n";

        cout << "Attempting driver cleanup after failed driver load, errors may follow.\n";
        unloadDriver();

        exit(2);
    }

    cout << " ...Success.\n";
}

// Unload the SMM IO driver.
void unloadDriver()
{
    cout << "Unloading SMM IO driver...\n";
    BDSID_Shutdown();
    cout << " ...Done.\n";
}
