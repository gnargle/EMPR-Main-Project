#include "adc.c"

void distanceircalc(void);

char s[3] = "< 6";

void distanceircalc(void){
    int x;
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