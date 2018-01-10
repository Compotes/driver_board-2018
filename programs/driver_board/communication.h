#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "ch.h"
#include "hal.h"
#include "motor.h"

#define MOVE_COMMAND 32123
#define MOVE_COMMAND_1 125
#define MOVE_COMMAND_2 123

#define TURN_ON_LED 16384

extern thread_t *communicationp;

void communication_init(void);
void communication_thread(void);


#endif
