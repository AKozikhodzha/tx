#include <stdio.h>
#include <string.h>

#include "tx_to_mess.h"

typedef struct DecodedBytes
{
    char byte_a, byte_b;
} DecodedBytes;

int chek_bytes (char byte){
    if (((byte>=0x30) && (byte<=0x39))||((byte>=0x41)&&(byte<=0x46)))
    {
        return 0;
    }else
    {
        return -1;
    }
}
void ascii_decode (char *bytes, char *res_byte, int *err_code){
    *err_code=0;
    if (chek_bytes(bytes[0])!=0||chek_bytes(bytes[1])!=0)
    {
        *err_code = -1;
        return;
    }
    char byte_temp[3];
    sprintf(byte_temp, "%c%c", bytes[0], bytes[1]);
    char a =strtol(byte_temp, NULL, 16);
    *res_byte=a;
}
void ascii_encode (char byte, char *bytes){
    char byte_temp[3];
    sprintf(byte_temp, "%2X", byte);
    sprintf(bytes, byte_temp);
}

static void TTS_make_crc(MessToTx* message)
{
    int crc_counted = 0;
    char crc_temp[2];
    char crc_first_temp[2];
    for (size_t i = 2; i < message->buf_length; i++)
    {
        crc_counted = crc_counted ^ message->buf[i];
    }
    sprintf(crc_temp, "%X", crc_counted);
    message->buf_length = message->buf_length + 2;

    if (crc_counted < 16)
    {
        message->buf[message->buf_length - 2] = '0';
        message->buf[message->buf_length - 1] = crc_temp[0];
    }
    else 
    {
        message->buf[message->buf_length - 2] = crc_temp[0];
        message->buf[message->buf_length - 1] = crc_temp[1];
    }
}

void mess_to_tx(TxToMess* reverse, MessToTx *message, TxTs tx_ts){

    char dcd_bts[2];
    message->buf_length = 0;
    message->buf[0] = 'T';

    if (tx_ts==TX)
    {
        message->buf[1]=0x58;
    }
    if (tx_ts==TS)
    {
        message->buf[1]=0x53;
    }

    ascii_encode(reverse->ad, dcd_bts);     // AD
    message->buf[2] = dcd_bts[0];     
    message->buf[3] = dcd_bts[1];

    ascii_encode(reverse->id, dcd_bts);     // ID
    message->buf[4] = dcd_bts[0];     
    message->buf[5] = dcd_bts[1];    
    message->buf_length = 6;
    
    if (reverse->data_length != 0)          // DATA
    {
        for (size_t i = 0; i < reverse->data_length; i++)
        {
            ascii_encode(reverse->data[i], dcd_bts);
            message->buf[message->buf_length] = dcd_bts[0];
            message->buf[message->buf_length + 1] = dcd_bts[1];
            message->buf_length = message->buf_length + 2;
            size_t aa=i;
        }
    }
    
    if (tx_ts==TS)                              // CRC
    {
        TTS_make_crc(message);
    }

    message->buf[message->buf_length] = 0x0D;     // Suffix
    message->buf_length++;
    message->buf[message->buf_length] = 0x0A;
    message->buf_length++;
    message->buf[message->buf_length] = 0x00;
//конец кода дениса
    /*char *new_tx;
    new_tx[0]=0x54;
    if (tx_ts==TX)
    {
        new_tx[1]=0x58;
    }
    if (tx_ts==TS)
    {
        new_tx[1]=0x53;
    }
    new_tx[2]=reverse->ad;
    new_tx[3]=reverse->id;
    size_t i = 0; 
    for ( i; i <= sizeof(reverse->data); i++)
    {
        new_tx[i+4]=reverse->data[i];
    }
    if (tx_ts==TS)
    {
        TTS_make_crc(reverse);
        new_tx[i+7]="\r";
        new_tx[i+8]="\n";
    }else
    {
        new_tx[i+5]="\r";
        new_tx[i+6]="\n";
    }*/
    
    
    printf("\nmessage->buf: ");
    for (size_t i = 0; i <= message->buf_length; i++)
    {
        printf("%c", message->buf[i]);
    }
    printf("\n");
}


void main()
{
    char *bytes = "1F";
    char res_byte;
    int err_code;
    ascii_decode(bytes, &res_byte, &err_code);
    printf("%X\n", res_byte);
    char byte = 0x1F;
    char res_bytes[2];
    ascii_encode(byte, res_bytes);
    printf("%s\n", res_bytes);
    //начнем отсюда (сверху просто тест)
    //преобразуем message в тх
    char tx_message[] = {0x21, 0x01, 0x41, 0x35, 0x46, 0x1F, 0x3A, 0x41, 0x4F};
    size_t tx_packet_length = sizeof(tx_message)/sizeof(tx_message[0]);
    printf("tx_packet: %s \n", tx_message);
    
    TxToMess reverse;
    reverse.ad=tx_message[0];
    reverse.id=tx_message[1];
    reverse.data_length=tx_packet_length-2;
    reverse.data=malloc(reverse.data_length);
    size_t j = 0;
    for (size_t i = 2; i < tx_packet_length; i++)
    {
        reverse.data[j]=tx_message[i];
        j++;
    }
    MessToTx message;
    mess_to_tx(&reverse, &message, TS);

}