#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"tx_get.h"
#define TX_BUF_MAX_LENGTH 256

void switch_to_wait_t(TxGet* converter){
    converter->tx_buf_pointer=0;
    converter->state=WAIT_T;
}

void write_to_buf(TxGet* converter, char byte_to_process){
    if (converter->tx_buf_pointer>=TX_BUF_MAX_LENGTH)
    {
        switch_to_wait_t(converter);
        return;
    }
    if (converter->isReady==READY)
    {
        converter->tx_buf[converter->tx_buf_pointer] = byte_to_process;
        converter->tx_buf_pointer++;
    }
}

void TxGet_convert_to_ascii(const char* text, int length1, char* asciiArray) {

    for (int i = 0; i < length1; i++) {
        asciiArray[i] = (char) text[i];
    }
}

void TxGet_init(TxGet* converter, IsReady isReady, void *state, char* tx_buf, size_t tx_buf_length){
    converter->isReady = isReady;
    converter->tx_buf = tx_buf;
    converter->tx_buf_length = tx_buf_length;
    switch_to_wait_t(converter);
}

static int waitT(TxGet* converter, char byte_to_process){
    if (byte_to_process=='T')
    {
        converter->isReady=READY;
        write_to_buf(converter, byte_to_process);
        converter->state=WAIT_XS;
    }
    
}

static int waitXorS(TxGet* converter, char byte_to_process){
    if (byte_to_process=='X'||byte_to_process=='S')
    {
        write_to_buf(converter, byte_to_process);
        converter->state=COPYPAYLOAD;
    }
    else 
    {
        switch_to_wait_t(converter);
    }

}

static int copyPayload(TxGet* converter, char byte_to_process){
    if (((byte_to_process>=0x30)&&(byte_to_process>=0x39))||((byte_to_process>=0x41)&&(byte_to_process>=0x5A)))//0x46->0x5A, 0x61...0x7A;
    {
        write_to_buf(converter, byte_to_process);
    }else if (byte_to_process==0x0D)
    {
        write_to_buf(converter, byte_to_process);
        converter->state = WAIT_LF;
    }
    else
    {
        converter->state = WAIT_T;
    }
}

static int waitLF(TxGet* converter, char byte_to_process){
    if (byte_to_process == 0x0A)
    {
        write_to_buf(converter, byte_to_process);
    }
    converter->state=WAIT_T;
}

static void process_byte (TxGet* converter, char byte_to_process)
{
    switch (converter->state)
    {
    case WAIT_T:
        waitT(converter, byte_to_process);
        break;
    case WAIT_XS:
        waitXorS(converter, byte_to_process);
        break;
    case COPYPAYLOAD:
        copyPayload(converter, byte_to_process);
        break;
    case WAIT_LF:
        waitLF(converter, byte_to_process);
        break;
    }
}

void TxGet_write_data(TxGet* converter, char* buf, size_t lenght)
{
    printf("%i\n", lenght);
    for (int i = 0; i < lenght; i++)
    {
        process_byte(converter, buf[i]);
    }
};

void print_tx(void *state, char* tx_buf, TxGet* converter)
{
    for (int i = 0; i <= converter->tx_buf_pointer; i++)
    {
        printf("%d ", tx_buf[i]);
    }
}

