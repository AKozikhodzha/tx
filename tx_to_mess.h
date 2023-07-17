#ifndef TX_TO_MESS
#define TX_TO_MESS

#include <assert.h>

typedef enum TxTs{
    TX=0,
    TS,
} TxTs;

typedef struct TxToMess
{
    char ad;
    char id;
    size_t mess_length;
    char* message[256];
} TxToMess;

void TxToMess_init (TxToMess *tx_object, size_t length);
void tx_to_mess (char *input, size_t length, TxToMess *output);
void mess_to_tx(TxToMess* reverse, char *new_tx, TxTs tx_ts);
#endif