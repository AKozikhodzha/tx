#include <stdio.h>
#include <string.h>
#include "tx_to_mess.h"

void TxToMess_init (TxToMess* txstruct, char ad, char id, char* data, size_t data_length)
{
    txstruct->ad = ad;
    txstruct->id = id;
    txstruct->data = data;
    txstruct->data_length = data_length;
}

void MessToTx_init(MessToTx* msg,
     char* buf, size_t buf_lenght)
{
    msg->buf = buf;
    msg->buf_length = buf_lenght;
}


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
//преобразование тх пакета в объект
void tx_to_mess (char *tx_packet, size_t tx_packet_length, TxToMess *tx_object)
{
    int err_code=0;
    size_t new_length = tx_packet_length;
    tx_object->data_length = new_length-8;
    int data_lenght_temp = 0;
    
    char tx_ad[2];
    tx_ad[0]=tx_packet[2];
    tx_ad[1]=tx_packet[3];

    char tx_id[2];
    tx_id[0]=tx_packet[4];
    tx_id[1]=tx_packet[5];


    ascii_decode(tx_ad, &tx_object->ad, &err_code);
    ascii_decode(tx_id, &tx_object->id, &err_code);
    printf("ad: %2X,\n id: %2X,\n", tx_object->ad, tx_object->id);
    printf("data: ");
    if (tx_packet[1]=='X')
    {
        int j = 0;
        char buf_temp[new_length-6];
        char *temp=malloc(3);
        for ( int i=6; i < new_length-2; i+=2)
        {
            temp[0]=tx_packet[i];
            temp[1]=tx_packet[i+1];
            char a;
            ascii_decode(temp, &a, &err_code);
            buf_temp[j] = a;
            j++;
        }
        tx_object->data=buf_temp;
        
        printf("%s\n", tx_object->data);
        for (size_t k = 0; k < j; k++)
        {
            printf("%2X ", tx_object->data[k]);
        }
        printf("\n");
    }

    if (tx_packet[1]=='S')
    {
        int j = 0;
        char buf_temp[new_length-6];
        char *temp=malloc(3);
        for ( int i=6; i < new_length-4; i+=2)
        {
            temp[0]=tx_packet[i];
            temp[1]=tx_packet[i+1];
            char a;
            ascii_decode(temp, &a, &err_code);
            buf_temp[j] = a;
            j++;
        }
        tx_object->data=buf_temp;
        tx_object->data_length +=2;
        printf("%s\n", tx_object->data);
        for (size_t k = 0; k < j; k++)
        {
            printf("%2X ", tx_object->data[k]);
        }
        printf("\n");
    }else if (tx_packet[1]=='X'&&tx_packet[1]=='S')
    {
        printf("Error: tx packet not found!");
        return;
    }
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
    message->buf=malloc(100);
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
    //преобразуем тх в message
    char *tx_packet = "TX21014135461F3A414F\r\n";

    size_t tx_packet_length = strlen(tx_packet);
    TxToMess tx_object;
    tx_to_mess(tx_packet, tx_packet_length, &tx_object);
    TxToMess_init (&tx_object, tx_object.ad, tx_object.id, tx_object.data, tx_object.data_length);
    
    //преобразуем данные в тх
    char tx_message[] = {0x21, 0x01, 0x41, 0x35, 0x46, 0x1F, 0x3A, 0x41, 0x4F};
    size_t tx_message_length = sizeof(tx_message)/sizeof(tx_message[0]);
    
    TxToMess reverse;
    reverse.ad=tx_message[0];
    reverse.id=tx_message[1];
    reverse.data_length=tx_message_length-2;
    reverse.data=malloc(reverse.data_length);
    size_t j = 0;
    for (size_t i = 2; i < tx_message_length; i++)
    {
        reverse.data[j]=tx_message[i];
        j++;
    }
    MessToTx message;
    mess_to_tx(&reverse, &message, TS);
    MessToTx_init(&message, message.buf, message.buf_length);
    MessToTx new_mes;
    mess_to_tx(&tx_object, &new_mes, TX);
    MessToTx_init(&new_mes, new_mes.buf, new_mes.buf_length);
}