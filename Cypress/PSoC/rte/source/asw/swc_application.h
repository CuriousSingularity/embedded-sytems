#define timertickperiod 100

#include <math.h>

enum Runables_e
{
    RUN_READJOYSTICK = 0,
    RUN_CALCULATECONTROL,
    RUN_SETENGINE,
    RUN_SETBRAKELIGHT,
    // add the runable macros here
    RUN_MAX,
    RUN_SYSTEMSHUTDOWN,
};

#define ALL_RUNNABLES_ALIVE             (pow(2, RUN_MAX) - 1)

void run_readJoystick(void);

void run_calculateControl(void);

void run_setEngine(void);

void run_setBrakeLight(void);

void run_systemShutdown(void);