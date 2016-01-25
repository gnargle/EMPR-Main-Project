#include "rtc.c"
#include "lcd_display.c"
#include "keypad.c"
#include "IR.c"
//#include "largelcd.c"

int scan_mode(char previous);
int tape_measure_mode(char previous);
int calibration_mode(char previous);


int calibration_mode(char previous){
    write_usb_serial_blocking("calibrate\n\r", 11);
    while(1){
        char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
        else{
            distanceircalc();
            RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
            PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,18,PWM_MATCH_UPDATE_NOW);
            return 0;
        }
    }
}

int tape_measure_mode(char previous){
    write_usb_serial_blocking("tape measure\n\r", 14);
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        distanceircalc();
        RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
        PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,18,PWM_MATCH_UPDATE_NOW);
        return 1;
    }
}

int scan_mode(char previous){
    write_usb_serial_blocking("scan\n\r", 6);
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        distanceircalc();
        RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
        RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
        return 2;
    }
}

int multi_view_mode(char previous){
    write_usb_serial_blocking("multi view\n\r", 12);
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        distanceircalc();
        RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
        RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
        return 3;
    }
}

