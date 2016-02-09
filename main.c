#include "modes.c"

//NOBODY TOUCH MY INCLUDES THEY ARE DELICATE
#define usedi2c LPC_I2C1
#define i2cfunc 3
#define i2cport 0
#define i2cpin1 0
#define i2cpin2 1

int count2 = 0;
int mode;
mode = 0;
char a;
char previous;

int main(void){
    serial_init();
    rtc_init();
    pwm_init();
    adc_init();
    systick_init();
    ultrasound();

    PINSEL_CFG_Type PinCfg;
    pin_settings(PinCfg, i2cfunc, 0, 0, i2cport, i2cpin1);
    pin_settings(PinCfg, i2cfunc, 0, 0, i2cport, i2cpin2);
    I2C_Init(usedi2c, 100000);
    I2C_Cmd(usedi2c, ENABLE);
    display_init(59);
    keypad_init(33);
    //lrg_screen_init(39);
    SYSTICK_IntCmd(DISABLE);
    calibration_mode(previous);
    while(1){
        a = read_keypad(33);
        switch(mode){
            case 0: mode = calibration_mode(previous); break;
            case 1: mode = tape_measure_mode(previous); break;
            case 2: mode = scan_mode(previous); break;
            case 3: mode = multi_view_mode(previous); break;
        }
        ir_dist_arr[array_counter] = ir_dist;
        ir_raw_arr[array_counter] = ir_raw;
        if (us_dist > 0 && us_dist < 500){
                us_dist_arr[array_counter] = us_dist;
        }
        us_raw_arr[array_counter] = us_raw;
    }
}

/*//
void RTC_IRQHandler(void){
    write_usb_serial_blocking("interrupt\n\r", 11);
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,count,PWM_MATCH_UPDATE_NOW);
    count++;
    if (count >=31){
        count = 8;
    }
    RTC_SetTime((LPC_RTC_TypeDef *)LPC_RTC, RTC_TIMETYPE_SECOND, 0);
    RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
    RTC_ClearIntPending((LPC_RTC_TypeDef *)LPC_RTC, RTC_INT_ALARM);
}*/
