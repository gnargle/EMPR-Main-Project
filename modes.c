#include "lcd_display.c"
#include "keypad.c"
#include "IR.c"
#include "rtc.c"
#include <string.h>
//#include "largelcd.c"

int scan_mode(char previous);
int tape_measure_mode(char previous);
int calibration_mode(char previous);


int calibration_mode(char previous){
    lcd_display_mode("Calibrate");
    while(1){
        char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        clear_display(59);
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
    lcd_display_mode("Tape Measure");
    
    //DISPLAYS IR VALUE ON LCD SECOND LINE
    int measure = distanceircalc();
    char *measure_out;
    if (measure == -1){
        measure_out = "<6";
    }
    else{
        sprintf(measure_out, "%i", measure);
    }
    int addr2 = 0x80 + 16;
    int i;
    for (i = 0; i < strlen(measure_out); i++){
        addr2 = alloc_lcd_addr(addr2, i, measure_out);
    }
    //END OF LCD DISPLAYING

    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        clear_display(59);
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
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        clear_display(59);
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
    lcd_display_mode("Scan");
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        clear_display(59);
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
        clear_display(59);
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
    lcd_display_mode("Multi View");
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        clear_display(59);
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

void lcd_display_mode(char* currentmode){
    int addr = 0x80;
    int i;
    for (i = 0; i < strlen(currentmode); i++){
        addr = alloc_lcd_addr(addr, i, currentmode);
    }
}

