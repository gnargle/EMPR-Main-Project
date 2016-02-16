#include "lcd_display.c"
#include "keypad.c"
#include "IR.c"
#include "rtc.c"
#include "pwm.c"
#include "systick.c"
#include <stdlib.h>
#include <string.h>
#include "timer.c"

int scan_mode(char previous);
int tape_measure_mode(char previous);
int calibration_mode(char previous);
void lcd_display_bottom_row();
void lcd_display_top_row(char* currentmode);
void sensor_changer(int* selector_value, char* previous_key);
void servoreset(void);
//Variables used in systick handler

int systick_count = 0;
int count = 8;
int turndir = 0;
int turnspeed = 50;
int sensor_selector = 0;
int servo_start = 8;
int servo_stop = 28;

int ir_dist = 0;
int ir_raw = 0;
int us_dist = 0;
int us_raw = 0;
int calibration_adjust = 0;
int calib_tracker = 0;
int calib_total = 0;
int calibrated_flag = 0;
int act_val;
int servoangle;
int sweep_num;

int time_arr[100];
int angle_arr[100];
int ir_dist_arr[100];
int ir_raw_arr[100];
int us_dist_arr[100];
int us_raw_arr[100];
int ir_avg;
int us_avg;
int array_counter = 0;
int samplerate = 2500;
int num = 0;
float x = 0;
float y = 0;

//////////////////////////////////////////////////////////////
//                  MAIN MODES                              //
//////////////////////////////////////////////////////////////

int calibration_mode(char previous){
    sensor_changer(&sensor_selector, &previous);
    count = 18;
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,18,PWM_MATCH_UPDATE_NOW);
    

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
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        int calib_arr[3];
        int ir_reported;
        int us_reported;
        int calib_val;
        int u = 0;
        char write[2];
        strcpy(write, "");
        while(calib_tracker <3){
            if (u == 1000){
                sensor_changer_cali_mode(&sensor_selector, &previous);
                int addr = 0x80;
                int i;
                for(i=0; i < strlen("Input actual dist: "); i++){
                    addr = alloc_lcd_addr(addr, i, "Input actual dist: ");
                }
                for(i=0; i < strlen(write); i++){
                    addr = alloc_lcd_addr(addr, i, write);
                }
                char x;
                x = read_keypad(33);
                if (x == previous){
                    previous = x;
                }
                else if (x != 'Z' && x != previous){
                    if (x == 'A'){
                        clear_display(59);
                        strcpy(write, "");
                        previous = x;
                    }
                    else if (isalpha(x) || x == '*' || isdigit(x)){
                        addr = 0x80;
                        append(write, x);
                        clear_display(59);
                        for(i=0; i < strlen("Input actual dist: "); i++){
                            addr = alloc_lcd_addr(addr, i, "Input actual dist: ");
                        }
                        for(i=0; i < strlen(write); i++){
                            addr = alloc_lcd_addr(addr, i, write);
                        }
                        previous = x;
                    }
                    else if(x == '#'){
                        clear_display(59);
                        int act_val = atoi(write);  
                        ir_reported = ir_dist;
                        char port[3] = "";
                        sprintf(port, "%i", ir_reported);
                        //write_usb_serial_blocking("ir_report: ", 11);
                        //write_usb_serial_blocking(port, 3);
                        //write_usb_serial_blocking("\n\r", 2); 
                        us_reported = us_dist;
                        char port1[3] = "";
                        sprintf(port1, "%i", us_reported);
                        //write_usb_serial_blocking("us_report: ", 11);
                        //write_usb_serial_blocking(port1, 3);
                        //write_usb_serial_blocking("\n\r", 2); 
                        calib_val = (ir_reported - act_val);
                        //calib_val = ((ir_reported - act_val) + (us_reported - act_val))/2;
                        calib_arr[calib_tracker] = calib_val;
                        calib_tracker++;
                        strcpy(write, "");
                        previous = x;
                    }
                }
                else if (x == 'Z' && previous != 'Z'){
                    previous = 'Z';
                }
            }
            else{
                u++;
            }
        }
        if (calibrated_flag == 0){
            for (calib_tracker = 0; calib_tracker <3; calib_tracker++){
                calib_total += calib_arr[calib_tracker];
            }
            calibrated_flag = 1;
            calibration_adjust = calib_total/3;
            char port1[3] = "";
            sprintf(port1, "%i", calib_total);
            //write_usb_serial_blocking("total: ", 5);
            //write_usb_serial_blocking(port1, 3);
            //write_usb_serial_blocking("\n\r", 2);
            char port[3] = "";
            sprintf(port, "%i", calibration_adjust);
            //write_usb_serial_blocking("adjust: ", 6);
            //write_usb_serial_blocking(port, 3);
            //write_usb_serial_blocking("\n\r", 2);
        }

        lcd_display_top_row("Cali");
        lcd_display_bottom_row();
        //distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
        //keypad_change_sample_rate(&samplerate, a, &previous);
        average_calculator(us_dist_arr, ir_dist_arr, array_counter, &us_avg, &ir_avg);
        previous = keypad_check(a, previous);
        return 0;
    }
}

int tape_measure_mode(char previous){
    sensor_changer(&sensor_selector, &previous);
    count = 18;
    lcd_display_top_row("Tape");
    lcd_display_bottom_row();
    

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
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        //distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
        PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,18,PWM_MATCH_UPDATE_NOW);
        keypad_change_sample_rate(&samplerate, a, &previous);
        average_calculator(us_dist_arr, ir_dist_arr, array_counter, &us_avg, &ir_avg);
        previous = keypad_check(a, previous);
        return 1;
    }
}

int scan_mode(char previous){
    sensor_changer(&sensor_selector, &previous);
    lcd_display_top_row("Scan");
    lcd_display_bottom_row();
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
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        keypad_change_servo_speed(&turnspeed, a, &previous);
        keypad_change_servo_start_pos(&servo_start, a, &previous);
        keypad_change_servo_stop_pos(&servo_stop, a, &previous);
        keypad_change_sample_rate(&samplerate, a, &previous);
        average_calculator(us_dist_arr, ir_dist_arr, array_counter, &us_avg, &ir_avg);
        previous = keypad_check(a, previous);

        //distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
        //RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
        return 2;
    }
}

int multi_view_mode(char previous){
    sensor_changer(&sensor_selector, &previous);
    lcd_display_top_row("Mult");
    lcd_display_bottom_row();
    //use sweep_num to see what stage of 'turning' the object is at    

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
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        keypad_change_servo_speed(&turnspeed, a, &previous);
        keypad_change_servo_start_pos(&servo_start, a, &previous);
        keypad_change_servo_stop_pos(&servo_stop, a, &previous);
        keypad_change_sample_rate(&samplerate, a, &previous);
        average_calculator(us_dist_arr, ir_dist_arr, array_counter, &us_avg, &ir_avg);
        previous = keypad_check(a, previous);
        
        //distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
        //RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
        return 3;
    }
}

//////////////////////////////////////////////////////////////
//                  IR/US SENSOR SWITCHER                   //
//////////////////////////////////////////////////////////////

void sensor_changer(int* selector_value, char* previous_key){
    char b = read_keypad(33);
    if (b == '1' && *previous_key != b){
        clear_display(59);
        //char port[1];
        //sprintf(port, "%i", *selector_value);
        //write_usb_serial_blocking(port, 1);
        *previous_key = b;
        if (*selector_value == 0){
                *selector_value = 1;
                return;
        }
        else {
                *selector_value = 0;
                return;
        }        
    }         
}

void sensor_changer_cali_mode(int* selector_value, char* previous_key){
    char b = read_keypad(33);
    if (b == '*' && *previous_key != b){
        clear_display(59);
        char port[1];
        sprintf(port, "%i", *selector_value);
        //write_usb_serial_blocking(port, 1);
        *previous_key = b;
        if (*selector_value == 0){
                *selector_value = 1;
                return;
        }
        else {
                *selector_value = 0;
                return;
        }        
    }         
}

//////////////////////////////////////////////////////////////
//                  LCD DISPLAY FUNCTIONS                   //
//////////////////////////////////////////////////////////////

void lcd_display_top_row(char* currentmode){
    char reqspeedtodisplay[3];
    char samplespersweeptodisplay[5];
    int samples_per_s = (int)1/((float)samplerate/1000000);

    sprintf(reqspeedtodisplay,"%i",turnspeed);
    sprintf(samplespersweeptodisplay,"%i",samples_per_s);

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
    addr = alloc_lcd_addr(addr, 0, "/");
    addr = alloc_lcd_addr(addr, 0, "s");
}


void lcd_display_bottom_row(){
    char rawvaluetodisplay[4];
    char servoangletodisplay[3];
    char distancetodisplay[3];
    char avgdistancetodisplay[3];
    int rawvalue;
    int s = 7;
    //Calculating angle of servo
    servoangle = ((count-8) * 9);
    
    //gets data from adc or ultrasound depending on mode.
    //if (sensorselector == 0){}
    //}
    //else{
        //Ultrasound data
        //rawvalue = 0;
    //}
    //Writes <6 if distance too small

    if (sensor_selector == 0){

        rawvalue = ir_raw;

        int distance = ir_dist;
        if (distance == -1){ 
            sprintf(distancetodisplay, "<%i", s);  
        }
        else{  
            sprintf(distancetodisplay, "%i", distance);
        }              
        sprintf(rawvaluetodisplay,"%i",rawvalue);
        sprintf(servoangletodisplay,"%i",servoangle);
        sprintf(avgdistancetodisplay,"%i",ir_avg);

        int addr = 0x80+16;
        int i;
    //Displays all 4 things on the bottom row in correct order
        for (i = 0; i < strlen(rawvaluetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, rawvaluetodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(servoangletodisplay); i++){
            addr = alloc_lcd_addr(addr, i, servoangletodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(distancetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, distancetodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(avgdistancetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, avgdistancetodisplay);
        }
    }

    else{

        sprintf(distancetodisplay, "%i", us_dist);
        sprintf(rawvaluetodisplay,"%i",us_raw);
        sprintf(servoangletodisplay,"%i",servoangle);
        sprintf(avgdistancetodisplay,"%i",us_avg);

        int addr = 0x80+16;
        int i;
    //Displays all 4 things on the bottom row in correct order
        for (i = 0; i < strlen(rawvaluetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, rawvaluetodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(servoangletodisplay); i++){
            addr = alloc_lcd_addr(addr, i, servoangletodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(distancetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, distancetodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(avgdistancetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, avgdistancetodisplay);
        }
    }

    //int distance = distanceircalc();    
    
}

//////////////////////////////////////////////////////////////
//                  GENERAL FUNCTIONS                       //
//////////////////////////////////////////////////////////////

void average_calculator(int* us_arr, int* ir_arr, int counter, int* us_avg, int* ir_avg){

    int u;
    int us_total = 0;
    int ir_total = 0;
    if (array_counter > 100){
        clear_display(59);
        array_counter = 0;
        memset(ir_dist_arr, 0, sizeof(ir_dist_arr));
        memset(us_dist_arr, 0, sizeof(us_dist_arr));
        memset(ir_raw_arr, 0, sizeof(ir_raw_arr));
        memset(us_raw_arr, 0, sizeof(us_raw_arr));
        memset(time_arr, 0, sizeof(time_arr));
        memset(angle_arr, 0, sizeof(angle_arr));
        return;
    }
    else if (array_counter ==50){
        clear_display(59);
    }
    for (u = 0; u <= counter; u++){
        us_total += us_arr[u];
        ir_total += ir_arr[u];
    }
    *ir_avg = (ir_total/counter +1);
    *us_avg = (us_total/counter +1);
    return;
}

void append(char* s, char c){
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

void servoreset(void){
        turnspeed = 50;
        turndir = 0;
        systick_count = 0;
        count = 8;
        PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,count,PWM_MATCH_UPDATE_NOW);
        return;
}


//////////////////////////////////////////////////////////////
//                  INTERRUPT REQUEST HANDLERS              //
//////////////////////////////////////////////////////////////

void SysTick_Handler(void){
    if(turndir == 0){
        if (systick_count < turnspeed){
            systick_count++;
            return;
        }
        else{
            systick_count = 0;
            count++;
            PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,count,PWM_MATCH_UPDATE_NOW);
            if (count >=servo_stop){
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
            systick_count = 0;
            count--;
            PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,count,PWM_MATCH_UPDATE_NOW);
            if (count <=servo_start){
                //avgdistance = (sum / 20);
                turndir = 0;
            }
        }
    }
}

void TIMER0_IRQHandler(void){
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
    if (num == 0){
        num ++;
        GPIO_SetValue(2, pin);  
        TIM_UpdateMatchValue(LPC_TIM0, 0, 10);
    }
    else {
        num--;
        GPIO_ClearValue(2, pin);
        TIM_UpdateMatchValue(LPC_TIM0, 0, samplerate);
        ir_raw = get_data();
        ir_dist = distanceircalc(); + calibration_adjust;
        angle_arr[array_counter] = ((count-8) * 9);
        time_arr[array_counter] = RTC_GetTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND);
    }
    TIM_ResetCounter(LPC_TIM0);
}

void TIMER2_IRQHandler(void){
    TIM_ClearIntCapturePending(LPC_TIM2, TIM_CR0_INT);
    x = TIM_GetCaptureValue(LPC_TIM2, TIM_COUNTER_INCAP0);
}

void TIMER3_IRQHandler(void){
    TIM_ClearIntCapturePending(LPC_TIM3, TIM_CR1_INT);
    us_raw = TIM_GetCaptureValue(LPC_TIM3, TIM_COUNTER_INCAP1);
    float length = ((((us_raw_arr[array_counter] - x)/2)/29.1)*100);
    us_dist = length; + calibration_adjust;
    array_counter++;
    TIM_ResetCounter(LPC_TIM2);
    TIM_ResetCounter(LPC_TIM3);
}
