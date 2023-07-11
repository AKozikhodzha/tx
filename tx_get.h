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
    TxState state;
    size_t tx_buf_pointer;
    void(*send_tx) (void *state, char* tx_buf, size_t tx_buf_lenght);
    IsReady isReady;//flag 
    char* tx_buf;
    size_t tx_buf_length;
    void *send_tx_arg;
} TxGet;

void TxGet_init(TxGet* converter, void(*send_tx)(void *state, char* tx_buf, size_t tx_buf_length), void *send_tx_arg, char* tx_buf, size_t tx_buf_length);

void TxGet_write_data(TxGet* converter, char* buf, size_t lenght);

void TxGet_get_tx_paket(TxGet* converter, char* tx_buf, size_t *tx_buf_length);
void print_tx (void *state, char* tx_buf, size_t tx_buf_lenght);
void TxGet_test(TxGet* converter, char* test, size_t test_lenght);
#endif