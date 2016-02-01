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
float x = 0;
float y = 0;
int num = 0;
int* time_arr;
int* angle_arr;
int* ir_dist_arr;
int* us_dist_arr;
int array_counter = 0;


int main(void){
    serial_init();
    rtc_init();
    pwm_init();
    adc_init();
    systick_init();
    write_usb_serial_blocking("hi\n\r", 4);
    ultrasound();
    write_usb_serial_blocking("ho\n\r", 4);

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
        //get_data_and_print();
        a = read_keypad(33);
        //previous = keypad_check(a, previous);
        switch(mode){
            case 0: mode = calibration_mode(previous); break;
            case 1: mode = tape_measure_mode(previous); break;
            case 2: mode = scan_mode(previous); break;
            case 3: mode = multi_view_mode(previous); break;
        }
    }write_usb_serial_blocking("\n\r", 2);
}


void TIMER0_IRQHandler(void){
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
    ir_dist_arr[array_counter] = distanceircalc();
    angle_arr[array_counter] = ((count-8) * 9);
    time_arr[array_counter] = RTC_GetTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND);
    if (num == 0){
		num ++;
		GPIO_SetValue(2, pin);	
		TIM_UpdateMatchValue(LPC_TIM0, 0, 10);
	}
	else {
		num--;
		GPIO_ClearValue(2, pin);
		TIM_UpdateMatchValue(LPC_TIM0, 0, 200);
	}
    TIM_ResetCounter(LPC_TIM0);
}

void TIMER2_IRQHandler(void){
    TIM_ClearIntCapturePending(LPC_TIM2, TIM_CR0_INT);
    x = TIM_GetCaptureValue(LPC_TIM2, TIM_COUNTER_INCAP0);

}
void TIMER3_IRQHandler(void){
    TIM_ClearIntCapturePending(LPC_TIM3, TIM_CR1_INT);
    y = TIM_GetCaptureValue(LPC_TIM3, TIM_COUNTER_INCAP1);
    float length = ((((y - x)/2)/29.1)*100);
    us_dist_arr[array_counter] = length;
    array_counter++;
    char port[30] = "";
    sprintf(port, "Ultrasonic: %.2f", length);
    write_usb_serial_blocking(port, 30);
    write_usb_serial_blocking("\n\r", 2);
    TIM_ResetCounter(LPC_TIM2);
    TIM_ResetCounter(LPC_TIM3);
}
//
/*
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
