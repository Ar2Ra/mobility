/*
    Bluetooth communication on UART 1

    Bluetooth.c
*/

#include <stdio.h>
#include <string.h>
#include "Type.h"

#include "Bluetooth.h"

uint8 connected = 0;

extern void led_bits(uint8 set, uint8 bit);

void bt_server(void)
{
    fprintf(stderr, "AT+BTSCAN\r");
}

uint8 bt_connected(void)
{
    return connected;
}

void bt_command(uint8 *cmd)
{
    if (BT_ECHO) //Echo all bluetooth commands on UART0
        fprintf(stdout, "[BT] %s\r\n", cmd);

    if (strstr((const char *) cmd, "CONNECT"))
    {
        led_bits(1, 0); //set led0
        connected = 1;
    }
    
    if (strstr((const char *) cmd, "DISCONNECT"))
    {
        led_bits(0, 0); //clear led0
        connected = 0;
    }
}

uint8 bt_valid_cmd(uint8 *cmd)
{
    if ( (cmd[0] >= 'A' && cmd[0] <= 'Z') || (cmd[0] == '0') )
        return 1;

    return 0;
}
