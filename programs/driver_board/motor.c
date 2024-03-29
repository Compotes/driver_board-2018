<<<<<<< HEAD
#include "motor.h"

thread_t *motorp;
=======
#include "ch.h"
#include "hal.h"

#include "motor.h"

#define MOTOR_PWM_CHANNEL 3
>>>>>>> 35332c5802f1ee8be6816247d7e9dd8f43e2613f

void motor_forward(void) {
    palSetPad(MOTOR_IN_GPIO, MOTOR_IN_1);
    palClearPad(MOTOR_IN_GPIO, MOTOR_IN_2);
}

void motor_backward(void) {
    palSetPad(MOTOR_IN_GPIO, MOTOR_IN_2);
    palClearPad(MOTOR_IN_GPIO, MOTOR_IN_1);
}

void motor_stop(void) {
    palClearPad(MOTOR_IN_GPIO, MOTOR_IN_1);
    palClearPad(MOTOR_IN_GPIO, MOTOR_IN_2);
}

THD_WORKING_AREA(waMotorThread, 64);
THD_FUNCTION(MotorThread, arg) {
    (void)arg;

    int16_t motor_speed = 0;
<<<<<<< HEAD

=======
    
>>>>>>> 35332c5802f1ee8be6816247d7e9dd8f43e2613f
    thread_t *masterp;

    chRegSetThreadName("motor");

    while (1) {
        masterp = chMsgWait();
        motor_speed = (int16_t)chMsgGet(masterp);
        chMsgRelease(masterp, MSG_OK);

        if (motor_speed < 0) {
            motor_backward();
            pwmEnableChannel(&PWMD3, MOTOR_PWM_CHANNEL, -motor_speed);
        } else if(motor_speed > 0) {
            motor_forward();
            pwmEnableChannel(&PWMD3, MOTOR_PWM_CHANNEL, motor_speed);
        } else {
            motor_stop();
        }
    }

    chThdSleepMilliseconds(100);
}

static PWMConfig pwmcfg = {
    48000000, // 22kHz frequency
    2048, // 1000 ticks - period lenght
    NULL,
    {
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}
    },
    0,
    0
};

<<<<<<< HEAD
void motor_thread_init(void) {
=======
thread_t * motor_thread_init(void) {
>>>>>>> 35332c5802f1ee8be6816247d7e9dd8f43e2613f
    pwmInit();
    pwmStart(&PWMD3, &pwmcfg);

    palSetPadMode(MOTOR_IN_GPIO, MOTOR_IN_1, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(MOTOR_IN_GPIO, MOTOR_IN_2, PAL_MODE_OUTPUT_PUSHPULL);

    palSetPadMode(MOTOR_ENABLE_GPIO, MOTOR_ENABLE, PAL_MODE_ALTERNATE(1));

<<<<<<< HEAD
    motorp = chThdCreateStatic(waMotorThread, sizeof(waMotorThread),NORMALPRIO, MotorThread, NULL);
=======
    return chThdCreateStatic(waMotorThread, sizeof(waMotorThread),NORMALPRIO, MotorThread, NULL);
>>>>>>> 35332c5802f1ee8be6816247d7e9dd8f43e2613f
}
