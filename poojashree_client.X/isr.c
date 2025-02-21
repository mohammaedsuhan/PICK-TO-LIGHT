#include <xc.h>
#include "main.h"


void __interrupt() isr(void)
{
	if (INT0F == 1)
	{

        if(server_flag)
            interrupt_flag = !interrupt_flag;
        
        else
            receive_toggle_flag = !receive_toggle_flag;

        

		INT0F = 0;
	}
}


