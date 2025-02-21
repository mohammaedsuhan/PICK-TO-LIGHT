
#include "main.h"
#include "can.h"
#include <xc.h>
    
void __interrupt() isr(void)
{
    if (RCIF == 1)
    {
        if (OERR == 1)
            OERR = 0;
        
        ch = RCREG;
        RCIF = 0;
    }
}

