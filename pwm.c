//pwm port is 2
//pwm pins are 0-2, 3-5, mbed pins 28, 25, 24, 23, 22, 21
#include "lpc17xx_pwm.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"


#define pwmport 2
#define pwmpin 1
#define pwmfunc 1
#define pwmpin2 2

void pwm_init(int channel){
    PINSEL_CFG_Type PinCfg;
    PWM_TIMERCFG_Type PWMCfg;

    PWM_MATCHCFG_Type PWMMatchCfg;

    PWMCfg.PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
    PWMCfg.PrescaleValue = 85;

    PWM_Init((LPC_PWM_TypeDef *) LPC_PWM1, PWM_MODE_TIMER, &PWMCfg);

    pin_settings(PinCfg, pwmfunc, 0, 0, pwmport, channel-1);
    pin_settings(PinCfg, pwmfunc, 0, 0, pwmport, channel);

    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,0,256,PWM_MATCH_UPDATE_NOW);
    

    PWMMatchCfg.IntOnMatch = DISABLE;
    PWMMatchCfg.MatchChannel = 0;
    PWMMatchCfg.ResetOnMatch = ENABLE;
    PWMMatchCfg.StopOnMatch = DISABLE;

    PWM_ConfigMatch((LPC_PWM_TypeDef *) LPC_PWM1, &PWMMatchCfg);
    

    PWM_ChannelConfig((LPC_PWM_TypeDef *) LPC_PWM1, channel, PWM_CHANNEL_SINGLE_EDGE);
    PWM_ChannelConfig((LPC_PWM_TypeDef *) LPC_PWM1, channel+1, PWM_CHANNEL_SINGLE_EDGE);

    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,channel,0,PWM_MATCH_UPDATE_NOW);
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,channel+1,0,PWM_MATCH_UPDATE_NOW);
    
    PWMMatchCfg.IntOnMatch = DISABLE;
    PWMMatchCfg.MatchChannel = channel;
    PWMMatchCfg.ResetOnMatch = DISABLE;
    PWMMatchCfg.StopOnMatch = DISABLE;
    PWM_ConfigMatch((LPC_PWM_TypeDef *) LPC_PWM1, &PWMMatchCfg);
    PWM_ChannelCmd((LPC_PWM_TypeDef *) LPC_PWM1, channel, ENABLE);

    PWMMatchCfg.IntOnMatch = DISABLE;
    PWMMatchCfg.MatchChannel = channel+1;
    PWMMatchCfg.ResetOnMatch = DISABLE;
    PWMMatchCfg.StopOnMatch = DISABLE;
    PWM_ConfigMatch((LPC_PWM_TypeDef *) LPC_PWM1, &PWMMatchCfg);
    PWM_ChannelCmd((LPC_PWM_TypeDef *) LPC_PWM1, channel+1, ENABLE);

    PWM_ResetCounter((LPC_PWM_TypeDef *)LPC_PWM1);
    PWM_CounterCmd((LPC_PWM_TypeDef *)LPC_PWM1, ENABLE);    
}

void pwm_enable(void){
    PWM_Cmd((LPC_PWM_TypeDef *) LPC_PWM1, ENABLE);
}


