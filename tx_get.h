#ifndef TX_GET
#define TX_GET

typedef struct TxGet
{

} TxGet;
typedef enum IsReady {NOT_READY=0 ,READY} IsReady;

void TxGet_convert_to_ascii(const char* text, int length1, char* asciiArray);
void TxGet_write_data(TxGet* context, char* data, size_t data_length, IsReady* flagMes, char* newData, size_t* length);

#endif