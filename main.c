#include "adc.c"
#include "rtc.c"
#include "lcd_display.c"
#include "keypad.c"
//NOBODY TOUCH MY INCLUDES THEY ARE DELICATE
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
char s[3] = "< 6";

int scan_mode(void);
int tape_measure_mode(void);
int calibration_mode(void);
char keypad_check(char x, char prev);

void distanceircalc(void);

int main(void){
    serial_init();
    rtc_init();
    pwm_init(2);
    adc_init();
    distanceircalc();

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
        //ultrasound();
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
    count++;
    if (count >=31){
        count = 8;
    }
    RTC_SetTime((LPC_RTC_TypeDef *)LPC_RTC, RTC_TIMETYPE_SECOND, 0);
    RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
    RTC_ClearIntPending((LPC_RTC_TypeDef *)LPC_RTC, RTC_INT_ALARM);
}
void distanceircalc(void){
	int x;
	while(1){
	x = get_data_and_print();
	if (x <= 1250){
        	x = (((5461/(get_data_and_print()-17))-2)*10);
		//x = get_data_and_print();
		}
	else if (2400<= x && x <= 2420){
			x = 7;
		}
	else if (2380<= x && x <= 2400){
			x = 8;
		}
	else if (2200<= x && x <= 2380){
			x = 9;
		}
	else if (1900<= x && x <= 2200){
			x = 10;
		}
	else if (1800<= x && x <= 1900){
			x = 11;
		}
	else if (1700<= x && x <= 1800){
			x = 12;
		}
	else if (1600<= x && x <= 1700){
			x = 13;
		}
	else if (1550<= x && x <= 1600){
			x = 14;
		}
	else if (1500<= x && x <= 1550){
			x = 15;
		}
	else if (1450<= x && x <= 1500){
			x = 16;
		}
	else if (1400<= x && x <= 1450){
			x = 17;
		}
	else if (1350<= x && x <= 1400){
			x = 18;
		}
	else if (1300<= x && x <= 1350){
			x = 19;
		}
	else if (1250<= x && x <= 1300){
		x = 20;
		}

	//x = get_data_and_print();


	if (x <= 2420){
		char port[6] = "";
    		sprintf(port, "%i", x);
    		write_usb_serial_blocking(port, 6);
    		write_usb_serial_blocking("\n\r", 2);
		}
	else {
		char port[3] = "";
    		sprintf(port, "%s", s);
    		write_usb_serial_blocking(port, 3);
    		write_usb_serial_blocking("\n\r", 2);
	}
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
            RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 0);
            PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,17,PWM_MATCH_UPDATE_NOW);
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
        RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 0);
        PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,count,PWM_MATCH_UPDATE_NOW);
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
        RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
        return 2;
    }
}


