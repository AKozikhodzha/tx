#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"tx_get.h"
#define TX_BUF_MAX_LENGTH 256

static int char_to_int (char char_to_process_0, char char_to_process_1)
{
    int CTI_result = 0;
    int CTI_temp_multiple = 0;
    char CTI_temp_string[2] = {char_to_process_0, char_to_process_1};
    for (int j = 0; j < 2; j++)
    {
        switch (j)
        {
            case 0:
                CTI_temp_multiple = 16;
                break;
            case 1:
                CTI_temp_multiple = 1;
                break;
        }
        
        switch (CTI_temp_string[j])
        {
            case '1':
                CTI_result = CTI_result + (1 * CTI_temp_multiple);
                break;
            case '2':
                CTI_result = CTI_result + (2 * CTI_temp_multiple);
                break;
            case '3':
                CTI_result = CTI_result + (3 * CTI_temp_multiple);
                break;
            case '4':
                CTI_result = CTI_result + (4 * CTI_temp_multiple);
                break;
            case '5':
                CTI_result = CTI_result + (5 * CTI_temp_multiple);
                break;
            case '6':
                CTI_result = CTI_result + (6 * CTI_temp_multiple);
                break;
            case '7':
                CTI_result = CTI_result + (7 * CTI_temp_multiple);
                break;
            case '8':
                CTI_result = CTI_result + (8 * CTI_temp_multiple);
                break;
            case '9':
                CTI_result = CTI_result + (9 * CTI_temp_multiple);
                break;
            case 'A':
                CTI_result = CTI_result + (10 * CTI_temp_multiple);
                break;
            case 'B':
                CTI_result = CTI_result + (11 * CTI_temp_multiple);
                break;
            case 'C':
                CTI_result = CTI_result + (12 * CTI_temp_multiple);
                break;
            case 'D':
                CTI_result = CTI_result + (13 * CTI_temp_multiple);
                break;
            case 'E':
                CTI_result = CTI_result + (14 * CTI_temp_multiple);
                break;
            case 'F':
                CTI_result = CTI_result + (15 * CTI_temp_multiple);
                break;
        }
    }
    return CTI_result;
}

static void check_CRC (TxGet* converter)
{
    int CRC_counted = 0;
    if (converter->tx_buf[1] == 'S')
    {
        for (int i = 0; i < (converter->tx_buf_pointer - 6); i++)
        {
         CRC_counted = CRC_counted ^ converter->tx_buf[i + 2];
        }

        if (CRC_counted == char_to_int (converter->tx_buf[converter->tx_buf_pointer - 4],
          converter->tx_buf[converter->tx_buf_pointer - 3]))
        {
          converter->send_tx(converter->send_tx_arg, converter->tx_buf, converter->tx_buf_pointer);
        }
    }
    else if (converter->tx_buf[1] == 'X')
    {
        converter->send_tx(converter->send_tx_arg, converter->tx_buf, converter->tx_buf_pointer);
    }
}

void switch_to_wait_t(TxGet* converter){
    converter->state=WAIT_T;
    converter->tx_buf_pointer=0;
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

void TxGet_init(TxGet* converter, void(*send_tx)(void *state, char* tx_buf, size_t tx_buf_length), void *send_tx_arg, char* tx_buf, size_t tx_buf_length){
    converter->send_tx = send_tx;
    converter->send_tx_arg = send_tx_arg;
    converter->isReady = NOT_READY;
    converter->tx_buf = tx_buf;
    converter->tx_buf_length = tx_buf_length;
    switch_to_wait_t(converter);
}

static int waitT(TxGet* converter, char byte_to_process){
    for (size_t i = 0; i < sizeof(converter->tx_buf); i++)
    {
        converter->tx_buf[i] = 0x00;
    }
    if (byte_to_process == 'T') 
    {
        write_to_buf(converter, byte_to_process);
        converter->state = WAIT_XS;
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
    if (((byte_to_process>=0x30)&&(byte_to_process>=0x39))||((byte_to_process>=0x41)&&(byte_to_process>=0x46)))//0x46->0x5A, 0x61...0x7A;
    {
        write_to_buf(converter, byte_to_process);
    }else if (byte_to_process==0x0D)
    {
        write_to_buf(converter, byte_to_process);
        converter->state = WAIT_LF;
    }
    else
    {
        switch_to_wait_t(converter);
    }
}

static void waitLF(TxGet* converter, char byte_to_process){
    if (byte_to_process == 0x0A)
    {
        write_to_buf(converter, byte_to_process);
        if (converter->send_tx != NULL)
        {
            check_CRC(converter);
        }
    }
    switch_to_wait_t(converter);
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
    for (int i = 0; i < lenght; i++)
    {
        process_byte(converter, buf[i]);
    }
};

void TxGet_get_tx_paket(TxGet* converter, char* tx_buf, size_t *tx_buf_length)
{
    if (converter->tx_buf_length!=0)
    {
        /* code */
    }
    
};

void print_tx (void *state, char* tx_buf, size_t tx_buf_lenght)
{
    for (int i = 0; i < tx_buf_lenght - 1; i++)
    {
        printf("%c", tx_buf[i]);
    }
}

void TxGet_test(TxGet* converter, char* test, size_t test_lenght)
{
    printf("\n\n\nInput: ");
    for (int i = 0; i <= test_lenght - 1; i++)
    {
        printf("%c", test[i]);
    }
    printf("\nReceived: ");
    //DTX_write_data(converter, test, sizeof(test));
}
