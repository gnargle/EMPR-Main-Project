#include "lcd_display.c"
#include "keypad.c"
#include "IR.c"
//#include "rtc.c"
#include "pwm.c"
#include "systick.c"
#include <string.h>
#include "timer.c"
//#include "largelcd.c"

int scan_mode(char previous);
int tape_measure_mode(char previous);
int calibration_mode(char previous);
void lcd_display_bottom_row();
void lcd_display_top_row(char* currentmode);
void sensor_changer(void);

//Variables used in systick handler
int systick_count = 0;
int count = 8;
int turndir = 0;
int turnspeed = 2;
int avgdistance = 0;
int sensorselector = 0;

int calibration_mode(char previous){
    sensor_changer();
    count = 18;
    lcd_display_top_row("Calibration");
    lcd_display_bottom_row();
    

    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 4;
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
        return 4;
    }
}

int tape_measure_mode(char previous){
    sensor_changer();
    count = 18;
    lcd_display_top_row("Tpe Measure");
    lcd_display_bottom_row();
    

    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 4;
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
    sensor_changer();
    lcd_display_top_row("Scan");
    lcd_display_bottom_row();
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 4;
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
    sensor_changer();
    lcd_display_top_row("Multi View");
    lcd_display_bottom_row();
    
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 4;
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

void sensor_changer(void){
    char b = read_keypad(33);
    if (b == '1'){

        if (sensorselector = 0){
                sensorselector = 1;
        }
        else {
                sensorselector = 0;
        }        
    }         
}

void lcd_display_top_row(char* currentmode){
    char reqspeedtodisplay[2];
    char samplespersweeptodisplay[2];

    int samplespersweep = 20;

    sprintf(reqspeedtodisplay,"%i",turnspeed);
    sprintf(samplespersweeptodisplay,"%i",samplespersweep);

    int addr = 0x80;
    int i;
    for (i = 0; i < strlen(currentmode); i++){
        addr = alloc_lcd_addr(addr, i, currentmode);
    }
    addr = alloc_lcd_addr(addr, 0, " ");
    for (i = 0; i < strlen(reqspeedtodisplay); i++){
        addr = alloc_lcd_addr(addr, i, reqspeedtodisplay);
    }
    addr = alloc_lcd_addr(addr, 0, " ");
    for (i = 0; i < strlen(samplespersweeptodisplay); i++){
        addr = alloc_lcd_addr(addr, i, samplespersweeptodisplay);
    }
}


void lcd_display_bottom_row(){
    char rawvaluetodisplay[4];
    char servoangletodisplay[3];
    char distancetodisplay[3];
    char avgdistancetodisplay[3];
    int servoangle;
    int rawvalue;
    int s = 6;
    //Calculating angle of servo
    servoangle = ((count-8) * 9);
    
    //gets data from adc or ultrasound depending on mode.
    //if (sensorselector == 0){
    rawvalue = get_data();
    //}
    //else{
        //Ultrasound data
        //rawvalue = 0;
    //}
    //Writes <6 if distance too small
    int distance = distanceircalc();    
    if (distance == -1){ 
        sprintf(distancetodisplay, "<%i", s);  
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
    addr = alloc_lcd_addr(addr, 0, " ");
    for (i = 0; i < strlen(servoangletodisplay); i++){
        addr = alloc_lcd_addr(addr, i, servoangletodisplay);
    }
    addr = alloc_lcd_addr(addr, 0, " ");
    for (i = 0; i < strlen(distancetodisplay); i++){
        addr = alloc_lcd_addr(addr, i, distancetodisplay);
    }
    addr = alloc_lcd_addr(addr, 0, " ");
    for (i = 0; i < strlen(avgdistancetodisplay); i++){
        addr = alloc_lcd_addr(addr, i, avgdistancetodisplay);
    }
    addr = alloc_lcd_addr(addr, 0, " ");
}

void SysTick_Handler(void){
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
            if (count >=29){
                //avgdistance = (sum / 20);
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
                //avgdistance = (sum / 20);
                turndir = 0;
            }
        }
    }
}
