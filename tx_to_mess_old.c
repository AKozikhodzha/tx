#include <stdio.h>
#include <string.h>
#include "tx_to_mess.h"

void TxToMess_init (TxToMess *tx_object, char *mess, size_t length){
    tx_object->ad=0;
    tx_object->id=0;
    tx_object->mess_length=length;
    tx_object->message=mess;
}
/*void char_to_ascii(char* a, char b){
    char aa=a[0];
    char x = aa&0x0F;
    char y = (aa>>4)&0x0F;
    

    printf("a[01]= %s\n", x);
};*/

void mess_to_new (char *input1, int *ptr, char* new)
{
    int chek=0;
    for (size_t k = 2; k < *ptr-2; k++)
    {
        new[chek]=input1[k];
        chek++;
    }
    *ptr=chek;
}

void combine (char* combined, size_t chek, char* new){
    for (int i = 0; i < chek; i += 2) {
        combined[i/2] = ((new[i]*10)) + new[i+1];
        printf("\n[%d, %c][%d, %c][%d, %c]", new[i], new[i], new[i+1], new[i+1], combined[i/2], combined[i/2]);
    }
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
    *res_byte=strtol(byte_temp, NULL, 16);
}
void ascii_encode (char byte, char *bytes){
    char byte_temp[3];
    sprintf(byte_temp, "%.2X", byte);
    bytes[0]=byte_temp[0];
    bytes[1]=byte_temp[1];
}

void tx_to_mess (char *tx_packet, size_t tx_packet_length, TxToMess *tx_object)
{
    int i=0;
    output->ad = combined[i];
    output->id = combined[i+1];
    printf("\n\nad: %2x,\n id: %2x,\n message:\n", output->ad, output->id);
    i+=2;

    int j = 0;
    for ( i; i < length; i++)
    {
        output->message[j] = combined[i];
        printf("%c", output->message[j]);
        j++;
    }
}

static void TTS_make_crc(TxToMess* reverse)
{
    int crc_counted = 0;
    char crc_temp[2];
    char crc_first_temp[2];
    for (size_t i = 2; i < reverse->mess_length; i++)
    {
        crc_counted = crc_counted ^ *reverse->message[i];
    }
    sprintf(crc_temp, "%X", crc_counted);
    reverse->mess_length = reverse->mess_length + 2;

    if (crc_counted < 16)
    {
        reverse->message[reverse->mess_length - 2] = '0';
        reverse->message[reverse->mess_length - 1] = crc_temp[0];    
    }
    else 
    {
        reverse->message[reverse->mess_length - 2] = crc_temp[0];
        reverse->message[reverse->mess_length - 1] = crc_temp[1];
    }
}
void mess_to_tx(TxToMess* reverse, MessToTx *message, TxTs tx_ts){
    char *new_tx;
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
    for ( i; i <= sizeof(reverse->message); i++)
    {
        new_tx[i+4]=reverse->message[i];
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
    }
    
    
    printf("\nnew_tx = ");
    for (size_t i = 0; i <= sizeof(new_tx); i++)
    {
        printf("%c", new_tx[i]);
    }
    printf("\n");
}

void main()
{
    char *input1 = "TX2101203040\r\n";
    size_t input1_length=strlen(input1);
    char *mess[input1_length];
    printf("%d", input1_length);
    char* new[input1_length];
    char *mess = "56052";
    TxToMess output;
    TxToMess reverse;
    reverse.ad=0x21;
    reverse.id=0x32;
    for (size_t i = 0; i <= strlen(mess); i++)
    {
        reverse.data[i]=mess[i];
    }
    char* new_tx[256];
    
    TxToMess_init (&output, mess, input1_length);
    mess_to_new(input1, &input1_length, new);
    printf("%d", input1_length);

    char* combined[input1_length/2];
    combine (combined, input1_length, new);

    tx_to_mess(combined, input1_length/2, &output);
    mess_to_tx(&reverse, new_tx, TX);
    /*char* a[0]=17;
    char* b="7";
    printf("a=%s\n", a);
    printf("b=%s\n", b);

    char_to_ascii(a, b);
    printf("a=%s\n", a);*/
    
}