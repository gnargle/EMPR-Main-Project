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
        return prev;
    }
    else if (x == 'Z' && prev != 'Z'){
        return prev = 'Z';
    }
    else{
        return prev == "Z";
    }
}

void keypad_change_servo_speed(int* turn_speed, char input_key, char* previous_key){
    if (input_key == '*' && *previous_key != input_key){
        *previous_key = '*';
        switch(*turn_speed){
            case 2: *turn_speed = 8;return;
            case 8: *turn_speed = 15;return;
            case 15: *turn_speed = 25;return;
            case 25: *turn_speed = 50;return;
            case 50: *turn_speed = 100;return;
            case 100: *turn_speed = 2;return;
        }
    }
    else return;
}

void keypad_change_servo_start_pos(int* min_pos_num, char input_key, char* previous_key){
    if (input_key == '0' && *previous_key != input_key){
        *previous_key = '0';
        switch(*min_pos_num){
            case 7: *min_pos_num = 11;return;
            case 11: *min_pos_num = 15;return;
            case 15: *min_pos_num = 19;return;
            case 19: *min_pos_num = 7;return;
        }
    }
    else return;
}

void keypad_change_servo_stop_pos(int* max_pos_num, char input_key, char* previous_key){
    if (input_key == '#' && *previous_key != input_key){
        *previous_key = '#';
        switch(*max_pos_num){
            case 29: *max_pos_num = 25;return;
            case 25: *max_pos_num = 21;return;
            case 21: *max_pos_num = 17;return;
            case 17: *max_pos_num = 29;return;
        }
    }
    else return;
}

void keypad_change_sample_rate(int* sample_rate, char input_key, char* previous_key){
    if (input_key == '8' && *previous_key != input_key){
        *previous_key = '8';
        switch(*sample_rate){
            case 100: *sample_rate = 200;return;
            case 200: *sample_rate = 300;return;
            case 300: *sample_rate = 400;return;
            case 400: *sample_rate = 600;return;
            case 600: *sample_rate = 1000;return;
            case 1000: *sample_rate = 2000;return;
            case 2000: *sample_rate = 100;return;
        }
    }
    else return;
}