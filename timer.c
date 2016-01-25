#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
//#include "serial.c"

void pinsetup(void);
int rising(int);
int falling(int);
void timer_init(void);

void ultrasound(void){
  pinsetup();
  timer_init();
  int x=0;
  int y=0;
  write_usb_serial_blocking("c\n\r", 4);
  while (1){
    write_usb_serial_blocking("d\n\r", 4);
    if (TIM_GetIntCaptureStatus(LPC_TIM3, TIM_CR0_INT) == SET) { //works with RESET (which means it's not detecting an interupt but it should be)
      write_usb_serial_blocking("e\n\r", 4);
      rising(x);
      int a = 0;
      while (a==0){
        if (TIM_GetIntCaptureStatus(LPC_TIM3, TIM_CR1_INT) == SET) {
            falling(y);
            int length = y - x;
            char port[10] = "";
            sprintf(port, "%i", length);
            write_usb_serial_blocking(port, 10);
            write_usb_serial_blocking("\n\r", 2);
            TIM_ResetCounter(LPC_TIM3);
            a = 1;
        }
      }
    }
    //write_usb_serial_blocking("f\n\r", 4);
  }
}

void timer_init(void){
  TIM_TIMERCFG_Type TIMERCfg;
  TIM_CAPTURECFG_Type RISINGCfg;
  TIM_CAPTURECFG_Type FALLINGCfg;
  TIM_MATCHCFG_Type RISINGMatch;
  TIM_MATCHCFG_Type FALLINGMatch;
  
  TIMERCfg.PrescaleOption = TIM_PRESCALE_USVAL;
  TIM_Init(LPC_TIM3, TIM_TIMER_MODE, &TIMERCfg);

  TIM_ConfigStructInit(TIM_TIMER_MODE, &TIMERCfg);
  
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
  
  TIM_Cmd(LPC_TIM3, ENABLE);
  
  }
  
int rising(x){
  x = TIM_GetCaptureValue(LPC_TIM3, TIM_COUNTER_INCAP0);
  TIM_ClearIntCapturePending(LPC_TIM3, TIM_CR0_INT);
  return x;
  }
  
int falling(y){
  y = TIM_GetCaptureValue(LPC_TIM3, TIM_COUNTER_INCAP1);
  TIM_ClearIntCapturePending(LPC_TIM3, TIM_CR1_INT);
  return y;
  }
      
void pinsetup(void){
  PINSEL_CFG_Type PinCfg;
  pin_settings(PinCfg, 3, 0, 0, 0, 24);
  pin_settings(PinCfg, 3, 0, 0, 0, 23);
  }
