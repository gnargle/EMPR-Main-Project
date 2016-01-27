#include "lcd_display.c"
#include "keypad.c"
#include "IR.c"
#include "rtc.c"
#include "systick.c"
#include <string.h>
//#include "largelcd.c"

int scan_mode(char previous);
int tape_measure_mode(char previous);
int calibration_mode(char previous);
void lcd_display_bottom_row();
void lcd_display_top_row(char* currentmode);
//Variables used in systick handler
int systick_count = 0;
int count = 8;
int turndir = 0;
int turnspeed = 2;
int avgdistance;

int calibration_mode(char previous){
    lcd_display_top_row("Calibrate");
    lcd_display_bottom_row;
    while(1){
        char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
        else{
            distanceircalc();
            //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
            PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,18,PWM_MATCH_UPDATE_NOW);
            return 0;
        }
    }
}

int tape_measure_mode(char previous){
    lcd_display_top_row("Tape Measure");
    lcd_display_bottom_row;
    

    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
        PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,18,PWM_MATCH_UPDATE_NOW);
        return 1;
    }
}

int scan_mode(char previous){
    lcd_display_top_row("Scan");
    lcd_display_bottom_row;
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        SYSTICK_IntCmd(ENABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
        //RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
        return 2;
    }
}

int multi_view_mode(char previous){
    lcd_display_top_row("Multi View");
    lcd_display_bottom_row;
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        SYSTICK_IntCmd(ENABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
        //RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
        return 3;
    }
}

void lcd_display_top_row(char* currentmode){
    char *reqspeedtodisplay;
    char *samplespersweeptodisplay;

    int samplespersweep = 20;

    sprintf(reqspeedtodisplay,"%i",turnspeed);
    sprintf(samplespersweeptodisplay,"%i",samplespersweep);

    int addr = 0x80;
    int i;
    for (i = 0; i < strlen(currentmode); i++){
        addr = alloc_lcd_addr(addr, i, currentmode);
    }
    for (i = 0; i < strlen(reqspeedtodisplay); i++){
        addr = alloc_lcd_addr(addr, i, reqspeedtodisplay);
    }
    for (i = 0; i < strlen(samplespersweeptodisplay); i++){
        addr = alloc_lcd_addr(addr, i, samplespersweeptodisplay);
    }
}

void lcd_display_bottom_row(){
    char *rawvaluetodisplay;
    char *servoangletodisplay;
    char *distancetodisplay;
    char *avgdistancetodisplay;
    int servoangle;
    int rawvalue;
    //Calculating angle of servo
    servoangle = (count * 9);
    
    //gets data from adc
    rawvalue = get_data();

    //Writes <6 if distance too small
    int distance = distanceircalc();
    if (distance == -1){
        distancetodisplay = "<6";
    }
    else{
        sprintf(distancetodisplay, "%i", distance);
    }

    sprintf(rawvaluetodisplay,"%i",rawvalue);
    sprintf(servoangletodisplay,"%i",servoangle);
    sprintf(avgdistancetodisplay,"%i",avgdistance);

    int addr = 0x80+16;
    int i;
    //Displays all 4 things on the bottom row in correct order
    for (i = 0; i < strlen(rawvaluetodisplay); i++){
        addr = alloc_lcd_addr(addr, i, rawvaluetodisplay);
    }
    for (i = 0; i < strlen(servoangletodisplay); i++){
        addr = alloc_lcd_addr(addr, i, servoangletodisplay);
    }
    for (i = 0; i < strlen(distancetodisplay); i++){
        addr = alloc_lcd_addr(addr, i, distancetodisplay);
    }
    for (i = 0; i < strlen(avgdistancetodisplay); i++){
        addr = alloc_lcd_addr(addr, i, avgdistancetodisplay);
    }
}

void SysTick_Handler(void){
    int list[20];
    int sum; 
    int i;
    if(turndir == 0){
        if (systick_count < turnspeed){
            systick_count++;
            return;
        }
        else{
            write_usb_serial_blocking("interrupt systick\n\r", 19);
            systick_count = 0;
            PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,count,PWM_MATCH_UPDATE_NOW);
            count++;
            list[count] = distanceircalc();
            if (count >=29){
                for (i = 0;i<sizeof(list);i++) {
                        sum += list[i];
                }
                avgdistance = (sum / 20);
                turndir = 1;

            }
        }
    }
    else {
        if (systick_count < turnspeed){
            systick_count++;
            return;
        }
        else{
            write_usb_serial_blocking("interrupt systick\n\r", 19);
            systick_count = 0;
            PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,count,PWM_MATCH_UPDATE_NOW);
            count--;
            if (count <=7){
                for (i= 0;i<sizeof(list);i++) {
                        sum += list[i];
                }
                avgdistance = (sum / 20);
                turndir = 0;
            }
        }
    }
}

