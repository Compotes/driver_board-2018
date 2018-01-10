#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "ch.h"
<<<<<<< HEAD
#include "hal.h"
#include "communication.h"

#define MOTOR_PWM_CHANNEL 3

extern thread_t *motorp;

void motor_thread_init(void);
=======

thread_t *motor_thread_init(void);
>>>>>>> 35332c5802f1ee8be6816247d7e9dd8f43e2613f

#endif
