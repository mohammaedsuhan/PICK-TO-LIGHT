#include<xc.h>
#include "ssd_display.h"
#ifndef MAIN_H
#define	MAIN_H

int interrupt_flag=0;

unsigned char key;
int key_flag=1;


int sw3_flag = 0;               
int sw2_flag = 0;   
int sw3_exit_flag=0;
int inside_update_flag = 0;
int save_flag=0;

int update_save_flag=0;
int product_save_flag=0;

int sw3_1_flag=0;

int display_flag_update=0;
int display_flag_product=0;

int read_update_flag=1;
int read_product_flag=1;



int update_flag=1;
int receive_flag=1;

int receive_conform_flag=0;

int normal_client_flag=1;

unsigned char received_up_stk[10],received_pt_id[10];

char count=0;
char count1=0;
char count2=0;
char count3=0;

unsigned char digit[]={ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE};
unsigned char ssd[4] = {ZERO, ZERO, ZERO, ZERO};
unsigned char update[] = {UI, MINUS, SI, TI};
unsigned char product[] = {P, MINUS, I, D};


int receive_toggle_flag=1;
int receive_interrupt_flag=1;
int dp_flag=1;
//int old_display=1;
int server_flag=1;
int read_one_time=0;
int server_enter_flag=0;






void init_config(void);
void update_stock(void);
void update_stock1(void);
void update_stock2(void);
void update_stock3(void);
void update_stock_function(void);
void product_id_function(void);
void compare_check(void);


void store_data_update_stock(void);
void store_data_product_stock(void);
void read_data_update_stock(void);
void read_data_product_stock(void);
void write_zero(void);
void update_ssd(void);
void clear_ssd(void);
void update_values(void);
void can_receive_stock(void);
void received_data_from_server(void);
void compare_check(void);
void can_receive_data(void);
void update2_stk(void);





#endif	/* MAIN_H */

