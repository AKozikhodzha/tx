#include<stdio.h>
#include"tx_client.h"

void TxCln_init(TxClient* client, char ad, void (*send_command)(char* buf, size_t length)){

}
void TxCln_get_data(TxClient* client, char id, char* args, size_t args_len, char* data, size_t max_data_length, char* err){

}
void TxCln_write_data(TxClient* client, char* buf, size_t length){

}
int main(){
    return 0;
}

