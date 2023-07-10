#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"tx_get.h"
#define TX_BUF_MAX_LENGTH 256

void main(void)
{
    TxGet converter;
    IsReady isReady = NOT_READY;
    char tx_buf_temp[TX_BUF_MAX_LENGTH];
    
    const char* text = "TWORLTXHELLO\r\nGGFTXWORLD!\r\nLDHI";
    size_t length = strlen(text);

    TxGet_init(&converter, isReady, NULL, tx_buf_temp, length);

    char* asciiArray = (char*) malloc(length * sizeof(char));

    TxGet_convert_to_ascii(text, length, asciiArray);

    printf("asciiArray=");
    for (int i = 0; i < length; i++){
        printf("%d ", asciiArray[i]);
    }
    printf("\n");

    TxGet_write_data(&converter, asciiArray, length);
    
    print_tx(NULL, tx_buf_temp, &converter);
}
