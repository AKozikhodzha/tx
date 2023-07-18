#include <stdio.h>
#include <string.h>

#include "tx_to_mess.h"

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
    tx_object->data_length = 0;
    int data_lenght_temp = 0;
    
    char tx_ad[2];
    tx_ad[0]=tx_packet[2];
    tx_ad[1]=tx_packet[3];

    char tx_id[2];
    tx_id[0]=tx_packet[4];
    tx_id[1]=tx_packet[5];


    ascii_decode(tx_ad, &tx_object->ad, &err_code);
    ascii_decode(tx_id, &tx_object->id, &err_code);
    printf("\n\nad: %2X,\n id: %2X,\n", tx_object->ad, tx_object->id);
    printf("data: ");
    if (tx_packet[1]=='X')
    {
        tx_object->TxOrTs=TX;
        
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
        printf("%s    ", tx_object->data);
        for (size_t k = 0; k < j; k++)
        {
            printf("%2X ", tx_object->data[k]);
        }
        

    }
    if (tx_packet[1]=='S')
    {
        tx_object->TxOrTs=TS;
        
        int j = 0;
        for ( int i=6; i < new_length-2; i++)
        {
            tx_object->data[j] = tx_packet[i];
            printf("%c", tx_object->data[j]);
            j++;
        }
    }else if (tx_packet[1]=='X'&&tx_packet[1]=='S')
    {
        printf("Error: tx packet not found!");
        return;
    }
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
    char *tx_packet = "TX21014135461F3A414F\r\n";
    printf("tx_packet: %s \n", tx_packet);
    size_t tx_packet_length = strlen(tx_packet);
    TxToMess tx_object;
    tx_to_mess(tx_packet, tx_packet_length, &tx_object);
    //преобразуем данные в тх
    
}