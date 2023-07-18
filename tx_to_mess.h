#ifndef TX_TO_MESS
#define TX_TO_MESS

#include <assert.h>
#include <stdlib.h>

typedef enum TxTs{
    TX=0,
    TS,
} TxTs;

typedef struct TxToMess
{
    char ad;
    char id;
    char* data;
    size_t data_length;
    TxTs TxOrTs;
} TxToMess;

typedef struct MessToTx
{
    char* buf;
    size_t buf_length;
} MessToTx;

void TxToMess_init (TxToMess* txstruct, char ad, char id, char* data, size_t data_length);

void tx_to_mess (char *input, size_t length, TxToMess *output);
void mess_to_tx(TxToMess* reverse, MessToTx *message, TxTs tx_ts);
#endif