#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"tx_get.h"

void TxGet_convert_to_ascii(const char* text, int length1, char* asciiArray) {

    for (int i = 0; i < length1; i++) {
        asciiArray[i] = (char) text[i];
    }
}

void TxGet_write_data(TxGet* context, char* data, size_t data_length, IsReady* isReady, char* newData, size_t* length)
{    
    int t1=0;
    for (int i = 0; i < data_length; i++) {
        if (data[i] == 0x54) {
            *isReady=READY;
            i++;

            if (i >= data_length || data[i] != 0x58)
                break;
        } else {
            continue;
        }

        loop:
        i++;

        while (i < *length) {
            if (data[i] == 0x0D && data[i + 1] == 0x0A)
            {    
                break;
            }
            newData[t1] = data[i];
            t1++;

            if (data[i+1] == 0x54 && data[i+2] == 0x58)
            {
                i=i+2;
                t1=0;
                memset(newData,0,sizeof(newData));
                goto loop;
            }else
            {
                i++;
                continue;
            }
            

        }
    }
    *length = t1;
};

void main(void)
{
    TxGet context;
    char testData[]={0x01, 0x54, 0x58, 0x08, 0x75, 0x0D, 0x02, 0x54, 0x58, 0x55, 0x54, 0x03, 0x04, 0x0D, 0x0A, 0x13, 0x10, 0x44, 0x54, 0x05};
    IsReady isReady = NOT_READY;
    char newData[100];
    size_t length = sizeof(testData);
    
    const char* text = "worldTXHello,TXworld!\r\nldhi";
    size_t length1 = strlen(text);

    // Создание нового массива для хранения ASCII-кодов
    char* asciiArray = (char*) malloc(length1 * sizeof(char));

    // Вызов функции для конвертации текста в ASCII-код
    TxGet_convert_to_ascii(text, length1, asciiArray);

 // Вывод ASCII-кодов
    for (int i = 0; i < length1; i++){
        printf("%d ", asciiArray[i]);
    }
    printf("\n");

    TxGet_write_data(&context, asciiArray, sizeof(asciiArray), &isReady, newData, &length1);

    if(isReady==READY)
    {
        //printf("testData = ");
        //for (int i=0; i< sizeof(testData); i++)
        //{
        //printf("%d ", testData[i]);
        //}
        //printf("\n");
        printf("newData = ");
        for (int i=0; i<length1; i++)
        {
        printf("%d ", newData[i]);
        }
        printf("\n");
        printf("%s\n", newData);
    }
}
