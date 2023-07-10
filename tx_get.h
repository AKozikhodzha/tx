#ifndef TX_GET
#define TX_GET

typedef enum IsReady {
    NOT_READY=0,
    READY} IsReady;

typedef enum TxState {
    WAIT_T=0, 
    WAIT_XS, 
    COPYPAYLOAD,
    WAIT_LF,
    } TxState;

typedef struct TxGet
{
    size_t tx_buf_pointer;
    TxState state;
    IsReady isReady;//flag 
    char* tx_buf;
    size_t tx_buf_length;
} TxGet;

void TxGet_init(TxGet* converter, IsReady isReady, void *state, char* tx_buf, size_t tx_buf_length);

void TxGet_convert_to_ascii(const char* text, int length1, char* asciiArray);
void TxGet_write_data(TxGet* converter, char* buf, size_t lenght);
void print_tx(void *state, char* tx_buf, TxGet* converter);

#endif