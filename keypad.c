//keypad port is 33
//write column - upper nibble
//read row - lower nibble
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "keypad.h"

#define usedi2c LPC_I2C1

void keypad_init(int i2c_port){
    //initialises the keypad on port specified by i2c_port.
    int init[1] = {0xFF};
    I2C_M_SETUP_Type TransferCfg;
    TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, init, 1, NULL, 0);
    I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
}

void write_keyboard_pin(uint8_t pin, int i2c_port){
    //determines key pressed (???)
    if (pin == 0){
        int buff[1] = {0xEF};
        I2C_M_SETUP_Type TransferCfg;
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, buff, 1, NULL, 0);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    }
    if (pin == 1){
        int buff[1] = {0xDF};
        I2C_M_SETUP_Type TransferCfg;
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, buff, 1, NULL, 0);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    }
    if (pin == 2){
        int buff[1] = {0xBF};
        I2C_M_SETUP_Type TransferCfg;
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, buff, 1, NULL, 0);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    }
    if (pin == 3){
        int buff[1] = {0x7F};
        I2C_M_SETUP_Type TransferCfg;
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, buff, 1, NULL, 0);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    }
}

unsigned char read_keypad_main(int i2c_port){
    //Main function that reads from the keypad by calling "write keyboard_pin" and then... (???)
    uint8_t i;
    for (i = 0; i < 4; i++){
        keypad_init(i2c_port);
        write_keyboard_pin(i, i2c_port);
        unsigned char sendbuff[1] = {0xDF};
        unsigned char receivebuff[1];
        I2C_M_SETUP_Type TransferCfg;
        //TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, sendbuff, 1, NULL, 0);
        //I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
        int j;
        for (j = 0; j <100000; j++){
            int x = 0;
        }
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, NULL, 0, receivebuff, 1);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
        char out;
        out = determine_key_pressed(TransferCfg.rx_data[0]);
        if (out != 'Z') {
            return out;
        }
    }
    return 'Z';
}

char read_keypad(int i2c_port){
    unsigned char retint;
    return read_keypad_main(i2c_port);
    //return determine_key_pressed(retint);
}

char determine_key_pressed(unsigned char retint){
    //lookup values for each potential value given by read keypad.
    switch (retint){
        case 0x77: return '1';
        case 0xB7: return '2';
        case 0xD7: return '3';
        case 0xE7: return 'A';
        case 0x7B: return '4';
        case 0xBB: return '5';
        case 0xDB: return '6';
        case 0xEB: return 'B';
        case 0x7D: return '7';
        case 0xBD: return '8';
        case 0xDD: return '9';
        case 0xED: return 'C';
        case 0x7E: return '*';
        case 0xBE: return '0';
        case 0xDE: return '#';
        case 0xEE: return 'D';
        default: return 'Z';
    }
}
