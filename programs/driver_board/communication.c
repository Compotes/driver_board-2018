#include "communication.h"

thread_t *communicationp;

int16_t speed;
int16_t received_command;


THD_WORKING_AREA(waSerialThread, 128);
THD_FUNCTION(SerialThread, arg) {
	(void)arg;

	received_command = sdGet (&SD1);
    sdPut (&SD1, received_command);
    while(received_command != 'A'){
		received_command = sdGet (&SD1);
		sdPut (&SD1, received_command);
	}

    while (1) {
		while(received_command != MOVE_COMMAND_1){
			received_command = sdGet (&SD1);
			sdPut (&SD1, received_command);
		}
		received_command = (received_command << 8) | sdGet (&SD1);
		//sdPut (&SD1, *((uint8_t*)&(received_command)+1));
		sdPut (&SD1, *((uint8_t*)&(received_command)+0));

        if (received_command == MOVE_COMMAND) {
			speed = sdGet (&SD1);
			speed = (speed << 8) | sdGet (&SD1);
			if (speed >= TURN_ON_LED || speed <= -TURN_ON_LED) {
				led (GREEN, 1);
			} else {
				led (GREEN, 0);
			} if (speed >= TURN_ON_LED) {
				speed -= TURN_ON_LED;
			} else if (speed <= -TURN_ON_LED) {
				speed += TURN_ON_LED;
			}
			chMsgSend (motorp, (msg_t)speed);
		}
	}
}

static const SerialConfig serial_cfg = {
    115200, // baud rate
    0,
    0,
    0
};

void communication_init(void) {
    sdInit();
    sdStart(&SD1, &serial_cfg);
}

void communication_thread(void) {
    communicationp = chThdCreateStatic(waSerialThread, sizeof(waSerialThread),
                                       NORMALPRIO, SerialThread, NULL);
}
