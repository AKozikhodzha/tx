#include<stdio.h>
#include<stdlib.h>
#include <string.h>

void replace_with_ascii(char* arr, int length, char* one) {
    for (int i = 0; i < length; i++) {
        one[i] = (int)arr[i];
    }
}

void asciiToHex(char* asciiCodes, int size, char* hexResult) {
    for(int i = 0; i < size; i++) {
        sprintf(hexResult + (i * 2), "%02X", asciiCodes[i]);
    }
}



