#ifndef TX_TO_MESS
#define TX_TO_MESS

#include <assert.h>

typedef struct TxToMess
{
    char ad;
    char id;
    char message[25];
} TxToMess;

void TxToMess_init (TxToMess *tx_object);
void sort_tx_struct (char *input, TxToMess *output);
void mess_to_tx(TxToMess* reverse, char *new_tx);
#endif