
// Header files
#include <math.h>

#define timertickperiod             100
#define MAX_JOYSTICK_VALUE          122

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

/**
 * The runnable which reads the Joystick values
 * @para void
 * @return void
 */
void run_readJoystick(void);

/**
 * The runnable calculates the engine speed based on the acceleration pedel
 * @para void
 * @return void
 */
void run_calculateControl(void);

/**
 * The runnable which sets the engine speed
 * @para void
 * @return void
 */
void run_setEngine(void);

/**
 * The runnable which sets the brake light based on acceleration pedel
 * @para void
 * @return void
 */
void run_setBrakeLight(void);
