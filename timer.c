#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"

void pinsetup(void);

void timer_init(void){
  pinsetup();
  TIM_TIMERCFG_Type TIMERCfg;
  TIM_CAPTURECFG_Type RISINGCfg;
  TIM_CAPTURECFG_Type FALLINGCfg;
  TIM_MATCHCFG_Type RISINGMatch;
  TIM_MATCHCFG_Type FALLINGMatch;
  
  TIMERCfg.PrescaleOption = TIM_PRESCALE_USVAL;
  TIM_Init(LPC_TIM3, &TIM_TIMER_MODE);
  
  RISINGCfg.CaptureChannel = 0;
  RISINGCfg.FallingEdge = DISABLE;
  RISINGCfg.IntOnCaption = ENABLE;
  RISINGCfg.RisingEdge = ENABLE;
  FALLINGCfg.CaptureChannel = 1;
  FALLINGCfg.FallingEdge = ENABLE;
  FALLINGCfg.IntOnCaption = ENABLE;
  FALLINGCfg.RisingEdge = DISABLE;
  TIM_ConfigCapture(LPC_TIM3, &FALLINGCfg);
  TIM_ConfigCapture(LPC_TIM3, &RISINGCfg);
  
  /* RISINGMatch.ExtMatchOutputTpe = TIM_EXTMATCH_NOTHING;
  RISINGMatch.IntOnMatch = ENABLE;
  RISINGMatch.MatchChannel = 0;
  RISINGMatch.ResetOnMatch = DISABLE;
  RISINGMatch.StopOnMatch = DISABLE;
  FALLINGMatch.ExtMatchOutputTpe = TIM_EXTMATCH_NOTHING;
  FALLINGMatch.IntOnMatch = ENABLE;
  FALLINGMatch.MatchChannel = 1;
  FALLINGMatch.ResetOnMatch = DISABLE;
  FALLINGMatch.StopOnMatch = DISABLE;
  TIM_ConfigMatch(LPC_TIM3, &RISINGMatch);
  TIM_ConfigMatch(LPC_TIM3, &FALLINGMatch); */
  
  TIM_Cmd(LPC_TIM0, ENABLE);
  
  }
  
int rising(x){
  if (TIM_GetIntCaptureStatus(LPC_TIM3, TIM_CR0_INT)) {
    x = TIM_GetCaptureValue(LPC_TIM3, TIM_COUNTER_INCAP0);
    TIM_ClearIntCapturePending(LPC_TIM3, TIM_CR0_INT);
    return x;
    }
  }
  
int falling(y){
  if (TIM_GetIntCaptureStatus(LPC_TIM3, TIM_CR1_INT)) {
    y = TIM_GetCaptureValue(LPC_TIM3, TIM_COUNTER_INCAP1);
    TIM_ClearIntCapturePending(LPC_TIM3, TIM_CR1_INT);
    return y;
    }
  }
      
void pinsetup(void){
  PINSEL_CFG_Type PinCfg;
  pin_settings(PinCfg, 3, 0, 0, 0, 24);
  pin_settings(PinCfg, 3, 0, 0, 0, 23);
  }
