#include "ch.h"
#include "hal.h"

#include <icu_lld.h>
#include "chprintf.h"
#include <inttypes.h>

#include "communication.h"

#define MOTOR_PWM_CHANNEL 3

#define UNUSED(arg) (void)(arg)

thread_t *commanderp;

int16_t motor_speed = 0;

void execute_master_command(uint16_t command_id, uint8_t *buff) {
    switch (command_id) {
        case 0xAA:  
            led(GREEN, buff[0]);
            break;

        case 0xBB:
            // Motor movement command
            motor_speed = (buff[1] * 1000 + buff[2] * 100 + buff[3] * 10 + buff[4]);
            if(buff[0] == 1) motor_speed *= -1;

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
    UNUSED(arg);
    while (1) { 
        if (motor_speed < 0) {
            motor_backward();
            pwmEnableChannel(&PWMD3, MOTOR_PWM_CHANNEL, -motor_speed);
        } else if(motor_speed > 0) {
            motor_forward();
            pwmEnableChannel(&PWMD3, MOTOR_PWM_CHANNEL, motor_speed);
        } else {
            motor_stop();
        }
        chThdSleepMilliseconds(100);
    }
    chRegSetThreadName("motor");
    chThdSleepMilliseconds(100);
}

THD_WORKING_AREA(waCommanderThread, 128);
THD_FUNCTION(CommanderThread, arg) {
    UNUSED(arg);

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
}

static PWMConfig pwmcfg = {
    22000, // 22kHz frequency
    1000, // 1000 ticks - period lenght
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


int main(void) {    
    halInit();
    chSysInit();

    /*
     * Communication init
     */

    communication_init();

    /*
     * PWM setup
     */  

    pwmInit();
    pwmStart(&PWMD3, &pwmcfg);

    palSetPadMode(MOTOR_IN_GPIO, MOTOR_IN_1, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(MOTOR_IN_GPIO, MOTOR_IN_2, PAL_MODE_OUTPUT_PUSHPULL);
    
    palSetPadMode(MOTOR_ENABLE_GPIO, MOTOR_ENABLE, PAL_MODE_ALTERNATE(1));
    
    /*
     * Threads setup
     */

    chThdCreateStatic(waMotorThread, sizeof(waMotorThread),
                      NORMALPRIO, MotorThread, NULL);

    commanderp = chThdCreateStatic(waCommanderThread, sizeof(waCommanderThread),
                                   NORMALPRIO, CommanderThread, NULL);

    communication_thread(commanderp);

    while (1) {
        chThdSleepMilliseconds(100);
    }
}
