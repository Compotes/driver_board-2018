#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "ch.h"
#include "hal.h"
#include "communication.h"

#define MOTOR_PWM_CHANNEL 3

extern thread_t *motorp;

void motor_thread_init(void);

#endif
