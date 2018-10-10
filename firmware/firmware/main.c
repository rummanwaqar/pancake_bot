/*
 * firmware.c
 *
 * Created: 2018-10-07 2:39:58 PM
 * Author : rumman
 */ 

#include "defines.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "motor.h"

FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_RW);

Command_t parseCommand(char* input_string, int* value);

int main(void)
{
	// initialize
	stdout = &uart_stream;
	uart_init();
	motor_init();
	_delay_ms(50);
	sei();
	
	PORT(B) |= _BV(5);
	DDR(B) |= _BV(5);
	
	int motor1 = 0;
	int motor2 = 0;
	
    /* Replace with your application code */
    while (1) 
    {
		motor_set_speed(motor1, motor2);
		// read commands
		if(uart_input_available())
		{
			int value;
			Command_t cmd=parseCommand(uart_get_string(), &value);
			switch(cmd) {
				case CMD_M1:
					printf("=Motor1:%d\n", value);
					motor1 = value;
					break;
				case CMD_M2:
					printf("=Motor2:%d\n", value);
					motor2 = value;
					break;
				default:
					break;
			}
		}
		
		PORT(B) ^= _BV(5);
    }
}

Command_t parseCommand(char* input_string, int* value)
{
	char command[7];
	int temp_val;
	if(input_string[0] == '>')
	{
		if(sscanf(input_string+1, "%s %d", command, &temp_val ) )
		{
			if (strcmp(command, "m1") == 0)
			{
				if(temp_val <= MOTOR_MAX_EFFORT && temp_val >= MOTOR_MAX_EFFORT * -1)
				{
					*value = temp_val;
					return CMD_M1;
				}
			}
			else if (strcmp(command, "m2") == 0)
			{
				if(temp_val <= MOTOR_MAX_EFFORT && temp_val >= MOTOR_MAX_EFFORT * -1)
				{
					*value = temp_val;
					return CMD_M2;
				}
			}
		}
	}
	return CMD_NULL;
}

