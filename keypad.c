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

char keypad_check(char x, char prev){
    if (x == prev){
        return prev = x;
    }
    else if (x != 'Z' && x != prev){
        prev = x;
        char out[1];
        sprintf(out, "%c", x);
        write_usb_serial_blocking(out, 1);
        write_usb_serial_blocking("\n\r", 2);
        return prev;
    }
    else if (x == 'Z' && prev != 'Z'){
        return prev = 'Z';
    }
    else{
        return prev == "Z";
    }
}

int keypad_change_servo_speed(int turn_speed, char input_key, char previous_key){
    if (input_key == '*' && previous_key != input_key){
        switch(turn_speed){
            case 2: return 25;
            case 25: return 50;
            case 50: return 100;
            case 100: return 250;
            case 250: return 500;
            case 500: return 2;
        }
    }
    else return turn_speed;
}

int keypad_change_servo_start_pos(int min_pos_num, char input_key, char previous_key){
    if (input_key == '0' && previous_key != input_key){
        switch(min_pos_num){
            case 7: return 11;
            case 11: return 15;
            case 15: return 19;
            case 19: return 7;
        }
    }
    else return min_pos_num;
}

int keypad_change_servo_stop_pos(int max_pos_num, char input_key, char previous_key){
    if (input_key == '#' && previous_key != input_key){
        switch(max_pos_num){
            case 29: return 25;
            case 25: return 21;
            case 21: return 17;
            case 17: return 29;
        }
    }
    else return max_pos_num;
}
