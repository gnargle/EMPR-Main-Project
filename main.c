#include "rtc.c"
#include "lcd_display.c"
#include "keypad.c"
#include "adc.c"
#include "main.h"
#include "timer.c"

#define usedi2c LPC_I2C1
#define i2cfunc 3
#define i2cport 0
#define i2cpin1 0
#define i2cpin2 1

int count = 8;
int count2 = 0;
int i;
int mode;
char a;
char previous;

int main(void){
    serial_init();
    rtc_init();
    pwm_init(2);
    pwm_enable();
    adc_init();

    PINSEL_CFG_Type PinCfg;
    pin_settings(PinCfg, i2cfunc, 0, 0, i2cport, i2cpin1);
    pin_settings(PinCfg, i2cfunc, 0, 0, i2cport, i2cpin2);
    I2C_Init(usedi2c, 100000);
    I2C_Cmd(usedi2c, ENABLE);
    display_init(59);
    keypad_init(33);
    calibration_mode();
    while(1){
        //get_data_and_print();
        ultrasound();
        a = read_keypad(33);
        previous = keypad_check(a, previous);
        switch(mode){
            case 0: mode = calibration_mode(); break;
            case 1: mode = tape_measure_mode(); break;
            case 2: mode = scan_mode(); break;
        }
    }
}

void RTC_IRQHandler(void){
    write_usb_serial_blocking("interrupt\n\r", 11);
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,count,PWM_MATCH_UPDATE_NOW);
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,3,80,PWM_MATCH_UPDATE_NOW);
    count++;
    if (count >=31){
        count = 8;
    }
    RTC_SetTime((LPC_RTC_TypeDef *)LPC_RTC, RTC_TIMETYPE_SECOND, 0);
    RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
    RTC_ClearIntPending((LPC_RTC_TypeDef *)LPC_RTC, RTC_INT_ALARM);
}//

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
}

int calibration_mode(void){
    write_usb_serial_blocking("calibrate\n\r", 11);
    while(1){
        char a = read_keypad(33);
        if (a == 'B' && previous != a){
            char a = read_keypad(33);
            previous = keypad_check(a, previous);
            return 0;
        }
        else if (a == 'C'&& previous != a){
            char a = read_keypad(33);
            previous = keypad_check(a, previous);
            return 1;
        }
        else if (a == 'D'&& previous != a){
            char a = read_keypad(33);
            previous = keypad_check(a, previous);
            return 2;
        }
        else{
            return 0;
        }
    }
}

int tape_measure_mode(void){
    write_usb_serial_blocking("tape measure\n\r", 14);
    char a = read_keypad(33);
    if (a == 'B'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'C'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'D'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else{
        return 1;
    }
}

int scan_mode(void){
    write_usb_serial_blocking("scan\n\r", 6);
    char a = read_keypad(33);
    if (a == 'B'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'C'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'D'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else{
        return 2;
    }
}
