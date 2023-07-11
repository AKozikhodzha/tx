#include <stdio.h>
#include <assert.h>
#include "data_to_tx.h"

typedef enum TEST_RESULT
{
    OK = 0,
    FAILED,
} TEST_RESULT;

void send_tx_test (void *state, char* tx_buf, size_t tx_buf_lenght)
{
    char* buf = (char*) state;
    for (int i = 0; i < tx_buf_lenght; i++)
    {
        buf[i] = tx_buf[i];
    }
}

TEST_RESULT simple_test()
{
    char test[] = {0x20, 0x25, 0x54, 0x58, 0x32, 0x35, 0x39, 0x37, 0x30, 0x33, 0x0D, 0x0A};
    char test_result[] = "TX259703\r\n"; 
    DataToTX converter;
    char test_buf[100];
    char tx_buf[100];
    DTX_init(&converter, send_tx_test, test_buf, tx_buf, 100);
    DTX_write_data(&converter, test, sizeof(test));
    printf("\n\nSIMPLE TEST:\n\n");
    printf("test result = ");
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        printf("%02x ", test_result[i]);
    }
    printf("\n");
    printf("     tx_buf = ");
    
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        printf("%02x ", tx_buf[i]);
    }
    printf("\n");
    
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        if (test_result[i] != tx_buf[i])
        {
            return(FAILED);
        }
    } 
    return(OK);
}

TEST_RESULT CRC_err_test()
{
    char test[] = {0x20, 0x25, 0x54, 0x53, 0x32, 0x35, 0x39, 0x37, 0x30, 0x33, 0x0D, 0x0A};
    char test_result[] = "TX259703\r\n"; 
    DataToTX converter;
    char test_buf[100];
    char tx_buf[100];
    DTX_init(&converter, send_tx_test, test_buf, tx_buf, 100);
    DTX_write_data(&converter, test, sizeof(test));
    printf("\n\nCRC ERR TEST:\n\n");
    printf("test resul = ");
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        printf("%02x ", test_result[i]);
    }
    printf("\n");
    printf("    tx_buf = ");
    
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        printf("%02x ", tx_buf[i]);
    }
    printf("\n");
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        if (test_result[i] != tx_buf[i])
        {
            return(FAILED);
        }
    } 
    return(OK);
}

TEST_RESULT many_tx_test()
{
    char test[] = {0x20, 0x25, 0x54, 0x58, 0x32, 0x35, 0x39, 0x37, 0x30, 0x33, 0x0D, 0x0A, 0x54, 0x58, 0x33, 0x30, 0x37, 0x39, 0x35, 0x32, 0x0D, 0x0D, 0x20, 0x25, 0x54, 0x58, 0x32, 0x35, 0x39, 0x37, 0x30, 0x33, 0x0D, 0x0A};
    char test_result[] = "TX259703\r\nTX259703\r\n"; 
    DataToTX converter;
    char test_buf[100];
    char tx_buf[100];
    DTX_init(&converter, send_tx_test, test_buf, tx_buf, 100);
    DTX_write_data(&converter, test, sizeof(test));
    printf("\n\nMANY TX TEST:\n\n");
    printf("test result = ");
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        printf("%02x ", test_result[i]);
    }
    printf("\n");
    printf("     tx_buf = ");
    
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        printf("%02x ", tx_buf[i]);
    }
    printf("\n");
    
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        if (test_result[i] != tx_buf[i])
        {
            return(FAILED);
        }
    } 
    return(OK);
}

TEST_RESULT unfinished_tx_test()
{
    char test[] = {0x20, 0x25, 0x54, 0x58, 0x32, 0x35, 0x39, 0x37, 0x30, 0x33, 0x0D, 0x0A};
    char test_result[] = ""; 
    DataToTX converter;
    char test_buf[100];
    char tx_buf[100];
    DTX_init(&converter, send_tx_test, test_buf, tx_buf, 100);
    DTX_write_data(&converter, test, sizeof(test));
    printf("\n\nUNFINISHED TX TEST:\n\n");
    printf("test result = ");
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        printf("%02x ", test_result[i]);
    }
    printf("\n");
    printf("     tx_buf = ");
    
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        printf("%02x ", tx_buf[i]);
    }
    printf("\n");
    
    for (int i = 0; i < sizeof(test_result) - 1; i++)
    {
        if (test_result[i] != tx_buf[i])
        {
            return(FAILED);
        }
    } 
    return(OK);
}

void main ()
{
    assert(simple_test() == OK);
    assert(CRC_err_test() == FAILED);
    assert(many_tx_test() == OK);
    assert(unfinished_tx_test() == OK);
}