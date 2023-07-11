#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"tx_get.h"
#define TX_BUF_MAX_LENGTH 256

void main(void)
{

    TxGet converter;
    char tx_buf_temp[TX_BUF_MAX_LENGTH];
    TxGet_init(&converter, print_tx, NULL, tx_buf_temp, TX_BUF_MAX_LENGTH);

    //TxGet_write_data(&converter, test, sizeof(test));

    char test4[] = {0x32, 0x54, 0x58, 0x32, 0x31, 0x30, 0x30, 0x30}; 
    char test3[] = {0x20, 0x25, 0x54, 0x58, 0x32, 0x35, 0x39, 0x37, 0x30, 0x33, 0x0D, 0x0A};
    char test2[] = {0x30, 0x30, 0x33, 0x0D, 0x0A};
    char test1[] = {0x54, 0x53, 0x32, 0x31, 0x30};

    TxGet_test(&converter, test1, sizeof(test1));
    TxGet_write_data(&converter, test1, sizeof(test1));

    TxGet_test(&converter, test2, sizeof(test2));
    TxGet_write_data(&converter, test2, sizeof(test2));

    TxGet_test(&converter, test3, sizeof(test3));
    TxGet_write_data(&converter, test3, sizeof(test3));

    TxGet_test(&converter, test4, sizeof(test4));
    TxGet_write_data(&converter, test4, sizeof(test4));
    
    //TxGet_get_tx_paket(&converter, tx_buf_new, &new_length);

    /*if (new_length!=0){
        for (size_t i = 0; i < new_length; i++)
        {
            printf("%d", tx_buf_new[i]);
        }
    }else{
        printf("TX paket not found");
    }*/
}
