#include <stdio.h>
#include <string.h>
#include "tx_to_mess.h"

void TxToMess_init (TxToMess *tx_object){
    tx_object->ad=0;
    tx_object->id=0;
}

void sort_tx_struct (char *input, TxToMess *output)
{
    int i=2;
    output->ad = input[i];
    output->id = input[i+1];
    printf("\n\nad: %2x,\n id: %2x,\n message:\n", output->ad, output->id);
    i+=2;

    int j = 0;
    while (input[i] != '\r' || input[i + 1] != '\n')
    {
        output->message[j] = input[i];
        j++;
        i++;
    }
    printf("%s", output->message);
}

void mess_to_tx(TxToMess* reverse, char *new_tx){
    new_tx[0]=0x54;
    new_tx[1]=0x58;
    new_tx[2]=reverse->ad;
    new_tx[3]=reverse->id;
    size_t i = 0; 
    for ( i; i <= sizeof(reverse->message); i++)
    {
        new_tx[i+4]=reverse->message[i];
    }
    new_tx[i+5]="\r";
    new_tx[i+6]="\n";
    printf("\nnew_tx = ");
    for (size_t i = 0; i <= sizeof(new_tx); i++)
    {
        printf("%c", new_tx[i]);
    }
    printf("\n");
    
    
};

void main()
{
    char *input = "TX!156052\r\n";
    char *mess = "56052";
    TxToMess output;
    TxToMess reverse;
    reverse.ad=0x21;
    reverse.id=0x32;
    for (size_t i = 0; i <= sizeof(mess); i++)
    {
        reverse.message[i]=mess[i];
    }
    char *new_tx[256];

    TxToMess_init (&output);
    sort_tx_struct(input, &output);
    mess_to_tx(&reverse, new_tx);

}