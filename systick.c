#include "lpc17xx_systick.h"
#include "lpc_types.h"

void systick_init(void){
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_InternalInit(100);
    SYSTICK_Cmd(ENABLE);
}

