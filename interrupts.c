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
                sweep_num += 1;
                if (sweep_num > 3){sweep_num = 0;}
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
    float length = (((us_raw - x)/58.2)*100);
    us_dist = length + us_calibration_adjust;  
    ir_raw = get_data();
    ir_dist = distanceircalc() + ir_calibration_adjust;
    angle_arr[array_counter] = ((count-8) * 9);
    time_arr[array_counter] = RTC_GetTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND);
    array_counter++;
    TIM_ResetCounter(LPC_TIM2);
    TIM_ResetCounter(LPC_TIM3);
}
