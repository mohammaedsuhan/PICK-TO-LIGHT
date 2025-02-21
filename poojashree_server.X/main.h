
#ifndef MAIN_H
#define	MAIN_H

int recive_flag=0;
int inwhile=1;

unsigned char ch;

int flag=1;
//int count=0;
//int k=0;

int data1_index=0;
int data2_index=0;


int change_up_pd=1;

unsigned char data1[15]="0000";
unsigned char data2[15]="0000";

unsigned char update_stock[10];
unsigned char product_id[10];








unsigned char up_stk[10]="0000";
unsigned char pd_id[10]="0000";
int max=4;
int count=0;

void init_config(void);
void data(void);

#endif	/* MAIN_H */

