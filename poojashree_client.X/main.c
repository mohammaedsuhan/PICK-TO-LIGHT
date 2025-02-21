


/*NAME POOJASHREE MN
 DATE = 10-02-2025
 DESCRIPTION = PICK TO LIGHT*/
#include <xc.h>
#include "digital_keypad.h"
#include "ssd_display.h"
#include "external_interrupt.h"
#include "main.h"
#include "eeprom.h"
#include "can.h"

void init_config(void)
{
    init_digital_keypad();
    init_ssd_control(); 
    init_can();
    init_external_interrupt();
    GIE=1;
    PEIE=1;
    TRISB0=1;
}

void main(void) 
{
    init_config();
    while (1)
    {
        can_receive_data();
        if(interrupt_flag)
        {       
            if(key_flag)
            {
                key = read_digital_keypad(STATE_CHANGE);
            }
            else
            {
                key=read_digital_keypad(LEVEL);    
            }   
            if(normal_client_flag)
            {
                if((key==SWITCH3) && (update_save_flag==0) && (product_save_flag==0)) 
                    sw3_flag=!sw3_flag;
                if(sw3_flag)
                {
                    product_id_function();
                }
                else
                {
                        update_stock_function();
                }
            }
            if(receive_toggle_flag==1)
            {
                key_flag=0;
                static unsigned int delay=0;
                static unsigned int delay1=0;
                static int server_data=0;
                 if (server_enter_flag == 1) 
                 {
                    server_data = 0; 
                     for (unsigned int i = 0; i < 4; i++) 
                     {
                        if (received_up_stk[i] >= '0' && received_up_stk[i] <= '9') 
                        { 
                            server_data = (server_data * 10) + (received_up_stk[i] - '0');
                        } 
                    }
                    server_enter_flag = 0;
                }
                if (key == SWITCH1 && delay++==20) 
                {   
                    server_data++;
                    if (server_data > 9999) 
                    { 
                        server_data = 0;
                    }
                        delay=0;   
                }
                else if (key == SWITCH2 && delay1++==20) 
                {
                    if (server_data > 0) 
                    {
                        server_data--; 
                    } 
                    else 
                    {
                        server_data = 9999; 
                    }
                    delay1=0;
                    
                    
                }
                
                if(key==SWITCH3)
                {
                    write_internal_eeprom(0x10,(server_data/1000));
                    write_internal_eeprom(0x11, ((server_data/100)%10));
                    write_internal_eeprom(0x12,((server_data/10)%10));
                    write_internal_eeprom(0x13,(server_data%10));
                    
                    normal_client_flag=1;
                    server_flag=1;
                    
                    interrupt_flag=0;
                    
                    key_flag=1;
                    server_data=0;
                    
                }

    
                ssd[0] = digit[server_data / 1000];
                ssd[1] = digit[(server_data / 100) % 10];
                ssd[2] = digit[(server_data / 10) % 10];
                ssd[3] = digit[server_data % 10];

                
                display(ssd);
               
            }
            else
            {

                key_flag=1;
                if(read_one_time)
                {
                    count3=read_internal_eeprom(0x10);
                    count2=read_internal_eeprom(0x11);
                    count1=read_internal_eeprom(0x12);
                    count=read_internal_eeprom(0x13);
                    read_one_time=0;
                }
                
                
                if(key==SWITCH3)
                {
                    write_internal_eeprom(0x10,count3);
                    write_internal_eeprom(0x11,count2);
                    write_internal_eeprom(0x12,count1);
                    write_internal_eeprom(0x13,count);
                    
                    count3=0,count2=0,count1=0,count=0;
                    
                    normal_client_flag=1;
                    server_flag=1;
                    interrupt_flag=0;
                    read_one_time=1;
                    dp_flag=1;
                    key_flag=1;
                    
                    
                }
                
                if(key==SWITCH2)
                {          
                    
                    if (dp_flag < 4)
                    {
                        dp_flag++;
                    }
                    else
                    {
                        dp_flag = 1;
                    }
                }
                
                
                if(dp_flag==1)
                {
                    ssd[0] = digit[count3];
                    ssd[1] = digit[count2];
                    ssd[2] = digit[count1];
                    ssd[3] = digit[count] | DOT;
                    if (key == SWITCH1) 
                    {
                        if (count < 9)
                            count++;
                        else
                            count = 0;
                    }
                }
                else if(dp_flag==2)
                {
                    ssd[0] = digit[count3];
                    ssd[1] = digit[count2];
                    ssd[2] = digit[count1] | DOT;
                    ssd[3] = digit[count];
    

                    if (key == SWITCH1) 
                    {
                        if (count1 < 9)
                            count1++;
                        else
                            count1 = 0;
                    }
                    
                }
                else if(dp_flag==3)
                {
                    ssd[0] = digit[count3];
                    ssd[1] = digit[count2] | DOT;
                    ssd[2] = digit[count1];
                    ssd[3] = digit[count];
                    if (key == SWITCH1) 
                    {
                        if (count2 < 9)
                            count2++;
                        else
                            count2 = 0;
                    }
                    
                }
                else if(dp_flag==4)
                {
                    ssd[0] = digit[count3] | DOT;
                    ssd[1] = digit[count2];
                    ssd[2] = digit[count1];
                    ssd[3] = digit[count];
                    if (key == SWITCH1) 
                    {
                        if (count3 < 9)
                            count3++;
                        else
                            count3 = 0;
                    }
                    
                }
                display(ssd);
                        
            }
        }
       
        
    }
        
}

void can_receive_data(void)
{
    if(can_receive())
    {
        received_up_stk[0]=can_payload[D0+0];
        received_up_stk[1]=can_payload[D0+1];
        received_up_stk[2]=can_payload[D0+2];
        received_up_stk[3]=can_payload[D0+3];
        received_up_stk[4]='\0';
        interrupt_flag=1;
        normal_client_flag=0;
        receive_toggle_flag=1;
        server_flag=0;
        server_enter_flag=1;
        read_one_time=1; 
    }
}


void product_id_function(void)
{
    if(display_flag_product==0)
    {
        display(product);
    }
    if (product_save_flag==1 && key == SWITCH3)
    {
        sw2_flag=0;
        interrupt_flag=0;
        display_flag_product=0;
        display_flag_update=0;
        sw3_flag=0;
        product_save_flag=0;
        
        write_internal_eeprom(0x21,count3);
        write_internal_eeprom(0x22,count2);
        write_internal_eeprom(0x23,count1);
        write_internal_eeprom(0x24,count);
        count=0,count1=0,count2=0,count3=0;
        read_product_flag=1;          
    }
    if(key==SWITCH2)
    {
        product_save_flag=1;
            count3 = read_internal_eeprom(0x21);
            count2 = read_internal_eeprom(0x22);
            count1 = read_internal_eeprom(0x23);
            count = read_internal_eeprom(0x24);
        display_flag_product=1;
        
       if (sw2_flag < 4)
       {
          sw2_flag++;
       }
       else
       {
          sw2_flag = 1;
       }
    }
    
    if(display_flag_product==1)
    {
        if (sw2_flag == 1)
        {
            write_internal_eeprom(0x24,count);
            ssd[0] = digit[count3];
            ssd[1] = digit[count2];
            ssd[2] = digit[count1];
            ssd[3] = digit[count] | DOT;
            if (key == SWITCH1) 
            {
                if (count < 9)
                    count++;
                else
                    count = 0;
            }
        }
        else if (sw2_flag == 2)
        {
            write_internal_eeprom(0x23,count1);
            ssd[0] = digit[count3];
            ssd[1] = digit[count2];
            ssd[2] = digit[count1] | DOT;
            ssd[3] = digit[count];
            if (key == SWITCH1) 
            {
                if (count1 < 9)
                    count1++;
                else
                    count1 = 0;
            }
        }
        else if (sw2_flag == 3)
        {
            write_internal_eeprom(0x22,count2);
            ssd[0] = digit[count3];
            ssd[1] = digit[count2] | DOT;
            ssd[2] = digit[count1];
            ssd[3] = digit[count];
            if (key == SWITCH1) 
            {
                if (count2 < 9)
                    count2++;
                else
                    count2 = 0;
            }
        }
        else if (sw2_flag == 4)
        {
            write_internal_eeprom(0x21,count3);
            ssd[0] = digit[count3] | DOT;
            ssd[1] = digit[count2];
            ssd[2] = digit[count1];
            ssd[3] = digit[count];
            if (key == SWITCH1) 
            {
                if (count3 < 9)
                    count3++;
                else
                    count3 = 0;
            }
        }
        display(ssd);
    }  
}

void update_stock_function(void)
{
    if(display_flag_update==0)
    {
        display(update);
    }
    
    if (update_save_flag==1 && key == SWITCH3)
    {
        can_transmit();
        sw2_flag=0;
        interrupt_flag=0;
        display_flag_product=0;
        display_flag_update=0;
        sw3_flag=0;
        update_save_flag=0;
        write_internal_eeprom(0x10,count3);
        write_internal_eeprom(0x11,count2);
        write_internal_eeprom(0x12,count1);
        write_internal_eeprom(0x13,count);
        count=0,count1=0,count2=0,count3=0;
        read_update_flag=1;            
    }   
    if(key==SWITCH2)
    {
        update_save_flag=1;
        if(read_update_flag)
        {
            count3 = read_internal_eeprom(0x10);
            count2 = read_internal_eeprom(0x11);
            count1 = read_internal_eeprom(0x12);
            count = read_internal_eeprom(0x13);
            read_update_flag=0;   
        }       
        display_flag_update=1; 
       if (sw2_flag < 4)
       {
          sw2_flag++;
       }
       else
       {
          sw2_flag = 1;
       }
    }
    if(display_flag_update==1)
    {
        if (sw2_flag == 1)
        {
            update_stock();
        }
        else if (sw2_flag == 2)
        {
            update_stock1();
        }
        else if (sw2_flag == 3)
        {
            update_stock2();
        }
        else if (sw2_flag == 4)
        {
            update_stock3();
        }
        display(ssd);
    }
    
}

void received_data_from_server(void)
{
    static int once_read_can_data =1,num;
    if(once_read_can_data)
    {
        for(unsigned int i=0;i<4;i++)
        {
            num = (num * 10) + (received_up_stk[i]-48);
        }
        once_read_can_data=0;
    }
    if(key==SWITCH1)
    {
        num++;
        if(num>9999)
            num=0;
        else if(key==SWITCH3)
        {
            sw3_flag=0;
            display_flag_update=0;
            interrupt_flag=0;
            sw2_flag=0;
            receive_flag=1;
            update_flag=1;
            receive_conform_flag=0;
            write_internal_eeprom(0x10,(num/1000));
            write_internal_eeprom(0x11, ((num/100)%10));
            write_internal_eeprom(0x12,((num/10)%10));
            write_internal_eeprom(0x13,(num%10));
            num=0;
        }
        ssd[0]=digit[num/1000];
        ssd[1]=digit[(num/100)%10];
        ssd[2]=digit[(num/10)%10];
        ssd[3]=digit[num%10];
        display(ssd);
    }
}

void update2_stk(void)
{
    if(read_one_time)
    {
        count3 = read_internal_eeprom(0x10);
        count2 = read_internal_eeprom(0x11);
        count1 = read_internal_eeprom(0x12);
        count = read_internal_eeprom(0x13);
        read_one_time=0;
    }
    if(key==SWITCH3)
    {
        write_internal_eeprom(0x10,count3);
        write_internal_eeprom(0x11,count2);
        write_internal_eeprom(0x12,count1);
        write_internal_eeprom(0x13,count);
        interrupt_flag=0;
        normal_client_flag=1;
        display_flag_update=0;
        display_flag_product=0;
        
    }
    
    if(key==SWITCH2)
    {
       if (dp_flag < 4)
       {
          dp_flag++;
       }
       else
       {
          dp_flag = 1;
       }        
    }
    if(dp_flag==1)
        update_stock();
    else if(dp_flag==2)
        update_stock1();
    else if(dp_flag==3)
        update_stock2();
    else if(dp_flag==4)
        update_stock3();    
    display(ssd);       
}
void update_stock(void)
{
    ssd[0] = digit[count3];
    ssd[1] = digit[count2];
    ssd[2] = digit[count1];
    ssd[3] = digit[count] | DOT;
    if (key == SWITCH1) 
    {
        if (count < 9)
            count++;
        else
            count = 0;
    }
}
void update_stock1(void)
{
    ssd[0] = digit[count3];
    ssd[1] = digit[count2];
    ssd[2] = digit[count1] | DOT;
    ssd[3] = digit[count];
    if (key == SWITCH1) 
    {
        if (count1 < 9)
            count1++;
        else
            count1 = 0;
    }
    
    
}
void update_stock2(void)
{
    ssd[0] = digit[count3];
    ssd[1] = digit[count2] | DOT;
    ssd[2] = digit[count1];
    ssd[3] = digit[count];
    if (key == SWITCH1) 
    {
        if (count2 < 9)
            count2++;
        else
            count2 = 0;
    } 
}
void update_stock3(void)
{
    ssd[0] = digit[count3] | DOT;
    ssd[1] = digit[count2];
    ssd[2] = digit[count1];
    ssd[3] = digit[count];
    if (key == SWITCH1) 
    {
        if (count3 < 9)
            count3++;
        else
            count3 = 0;
    }
}



