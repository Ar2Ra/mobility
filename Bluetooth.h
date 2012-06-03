/*
    Bluetooth communication on UART 1

    Bluetooth.h
*/

#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#define BT_ECHO 1

//Set bluetooth on server mode (discoverable)
void bt_server(void);

//Is the bluetooth device connected to another device?
uint8 bt_connected(void);

//Bluetooth command handler
void bt_command(uint8 *cmd);

//Returns 1 if "cmd" is a valid bluetooth system command
uint8 bt_valid_cmd(uint8 *cmd);

//Set as connected for testing purposes
void bt_set_connected(void);

#endif  // __BLUETOOTH_H
