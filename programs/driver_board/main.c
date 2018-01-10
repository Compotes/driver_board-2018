#include "ch.h"
#include "hal.h"

#include "communication.h"
#include "motor.h"

/*thread_t *commanderp;


void execute_master_command(uint16_t command_id, uint8_t *buff) {
    uint8_t j;
    int16_t motor_speed = 0;
    switch (command_id) {
        case 0xAA:
            led(GREEN, buff[0]);
            break;

        case 0xBB:
            // Motor movement command
            motor_speed = 0;
            motor_speed |= buff[1];
            motor_speed |= buff[2] << 4;
            motor_speed |= buff[3] << 8;

            if(motor_speed != 0) motor_speed += MISTAKE;

            if(buff[0] == 1) motor_speed *= -1;

            chMsgSend(motorp, motor_speed);

        default:
            break;
    }

}

uint8_t *send_data_command(uint16_t command_id, uint8_t data_length, uint8_t *buff) {
    uint32_t values = 0;
    uint8_t i;
    switch (command_id) {
        default:
            break;
    }
    return buff;
}

THD_WORKING_AREA(waCommanderThread, 128);
THD_FUNCTION(CommanderThread, arg) {
	(void)arg;

    thread_t *serialp;
    message_data_t *messagep;
    uint16_t cmd, command_id;
    uint8_t data_buff[MAX_DATA_CHAR_LENGTH];
    uint8_t data_length, i, rw_bit;

    while (1) {
        serialp = chMsgWait();
        messagep = (message_data_t *)chMsgGet(serialp);

        cmd = messagep->operation;

        command_id = cmd & COMMAND_ID_MASK;
        rw_bit = ((cmd >> OPERATION_BIT_LENGTH) & 1) + 1;
        data_length = cmd >> (OPERATION_BIT_LENGTH + RW_BIT);

        if (rw_bit == PROCESS_DATA_FROM_MASTER) {
            for (i = 0; i < data_length; i++) {
                data_buff[i] = messagep->data[i];
            }
            execute_master_command(command_id, data_buff);
        } else {
            send_data_command(command_id, data_length, data_buff);
            for (i = 0; i < data_length; i++) {
                messagep->data[i] = data_buff[i];
            }
        }

        chMsgRelease(serialp, (msg_t)messagep);
    }
    chRegSetThreadName("commander");
    chThdSleepMilliseconds(100);
}*/

int main(void) {
    halInit();
    chSysInit();

    /*
     * Communication init
     */

    communication_init();

    /*
     * Threads setup
     */

    //commanderp = chThdCreateStatic(waCommanderThread, sizeof(waCommanderThread),
    //                               NORMALPRIO, CommanderThread, NULL);

    communication_thread();

    motor_thread_init();

    while (1) {
        chThdSleepMilliseconds(100);
    }
}
